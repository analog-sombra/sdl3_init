#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
struct Sprite
{
    SDL_Texture *texture;

    Sprite(SDL_Renderer *renderer, const std::string &file_path)
    {
        SDL_Surface *surface = SDL_LoadBMP(file_path.c_str());
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);
    }
    ~Sprite()
    {
        SDL_DestroyTexture(texture);
    }

    void Render(SDL_Renderer *renderer)
    {
        SDL_FRect dest_rect{
            .x = 0, .y = 0, .w = 800, .h = 600};
        SDL_RenderTexture(renderer, texture, nullptr, &dest_rect);
    }
};

struct CustomText
{
    TTF_Font *font;
    SDL_Texture *textTexture;
    SDL_Surface *textSurface;
    CustomText(SDL_Renderer *renderer, std::string text)
    {
        if (!TTF_Init())
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF_Init failed: %s", SDL_GetError());
        }
        font = TTF_OpenFont("./assets/candy.otf", 24.f);
        if (font == nullptr)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF_OpenFont failed: %s", SDL_GetError());
        }
        TTF_SetFontSize(font, 400);
        TTF_SetFontOutline(font, 10);
        textSurface = TTF_RenderText_Blended(font, text.c_str(), 0, SDL_Color{255, 255, 255, 255});
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    }

    ~CustomText()
    {
        SDL_DestroySurface(textSurface);
    }
    void Render(SDL_Renderer *renderer)
    {
        SDL_FRect textRect = SDL_FRect{10, 50, 200, 50};
        SDL_RenderTexture(renderer, textTexture, nullptr, &textRect);
    }
};
struct SDLApplication
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    // SDL_Surface *surface;
    int r = 200;
    int g = 100;
    int b = 50;
    bool running = true;
    bool fullscreen = false;

    Sprite *sprite;
    CustomText *text;
    SDLApplication(const char *title)
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init failed: %s", SDL_GetError());
        }
        window = SDL_CreateWindow(title, 800, 600, SDL_WINDOW_RESIZABLE);
        renderer = SDL_CreateRenderer(window, nullptr);
        // renderer = SDL_CreateRenderer(window, "vulkan");

        SDL_SetRenderLogicalPresentation(renderer, 800, 600, SDL_LOGICAL_PRESENTATION_LETTERBOX);

        if (window == nullptr)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow failed: %s", SDL_GetError());
        }

        SDL_Log(std::format("Renderer: {}", SDL_GetRendererName(renderer)).c_str());
        for (int i = 0; i < SDL_GetNumRenderDrivers(); ++i)
        {
            SDL_Log(std::format("Render Driver {}: {}", i, SDL_GetRenderDriver(i)).c_str());
        }

        sprite = new Sprite(renderer, "./assets/test.bmp");
        text = new CustomText(renderer, "Hello SDL3");

        // surface = SDL_LoadBMP("./assets/test.bmp");
        // if (surface == nullptr)
        // {
        //     SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_LoadBMP failed: %s", SDL_GetError());
        // }
    }

    ~SDLApplication()
    {
        TTF_Quit();
        delete sprite;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void Tick()
    {
        Input();
        Update();
        Render();
    }
    void Input()
    {
        const bool *keys = SDL_GetKeyboardState(nullptr);
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN)
            {
                SDL_Log("Key pressed: %s", SDL_GetKeyName(event.key.key));
                // SDL_Log("Key pressed: %d", event.key.key);

                if (event.key.key == SDLK_1)
                    r = 150, b = 50, g = 50;
                if (event.key.key == SDLK_2)
                    r = 50, b = 150, g = 50;
                if (event.key.key == SDLK_3)
                    r = 50, b = 50, g = 150;
                if (event.key.key == SDLK_0)
                    r = 200, b = 100, g = 50;
                if (event.key.key == SDLK_F11)
                {
                    SDL_SetWindowFullscreen(window, fullscreen);
                    fullscreen = !fullscreen;
                }
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                    SDL_Log("Mouse button pressed: Left");
                else if (event.button.button == SDL_BUTTON_RIGHT)
                    SDL_Log("Mouse button pressed: Right");
                else if (event.button.button == SDL_BUTTON_MIDDLE)
                    SDL_Log("Mouse button pressed: Middle");
                else
                    SDL_Log("Mouse button pressed: %d", event.button.clicks);
            }
        }

        if (keys[SDL_SCANCODE_L] == true)
        {
            SDL_Log("Key pressed: %s", SDL_GetKeyName(SDLK_ESCAPE));
        }

        float x, y;
        SDL_MouseButtonFlags state = SDL_GetMouseState(&x, &y);
        if (state & SDL_BUTTON_LEFT)
        {
            SDL_Log("Mouse position: (%f, %f)", x, y);
        }
    }

    void Update()
    {
        // Update game logic here
    }

    void Render()
    {
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderClear(renderer);
        sprite->Render(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderDebugText(renderer, 10, 10, "Hello SDL2!");
        text->Render(renderer);

        SDL_RenderPresent(renderer);

        // SDL_Surface *windowSurface = SDL_GetWindowSurface(window);
        // if (nullptr != windowSurface)
        // {

        //     SDL_BlitSurface(surface, nullptr, windowSurface, nullptr);
        //     SDL_UpdateWindowSurface(window);
        // }
    }

    void MainLoop()
    {
        Uint64 fps = 0;
        Uint64 fpsTimer = SDL_GetTicks();

        while (running)
        {
            Tick();

            fps++;

            Uint64 now = SDL_GetTicks();

            if (now - fpsTimer >= 1000)
            {
                // SDL_Log("FPS: %llu", fps);
                SDL_SetWindowTitle(window, std::format("Vulkan Window - FPS: {}", fps).c_str());

                fps = 0;
                fpsTimer = now;
            }
        }
    }
};

int main()
{

    SDLApplication app("Vulkan Window");
    app.MainLoop();
    return EXIT_SUCCESS;
}