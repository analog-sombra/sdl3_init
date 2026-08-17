// #define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL 0

// https://www.youtube.com/watch?v=SpspkiGChww&list=PLvv0ScY6vfd-RZSmGbLkZvkgec6lJ0BfX&index=17

// #include <SDL3/SDL_vulkan.h>
// #include <format>
// #include <vulkan/vulkan.h>
// #include <vulkan/vulkan_raii.hpp>

// using namespace std;
// class SDLException final : public std::runtime_error
// {
// public:
//     SDLException(const std::string &message) : std::runtime_error(std::format("SDL Error: {} -> {}", message, SDL_GetError()))
//     {
//     }
// };

// class App
// {
//     std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window{nullptr, SDL_DestroyWindow};
//     bool running{true};

// public:
//     App()
//     {
//         if (!SDL_Init(SDL_INIT_VIDEO))
//         {
//             throw SDLException("SDL_Init failed");
//             // SDL_Log("SDL_Init failed: %s", SDL_GetError());
//             return;
//         }

//         if (!SDL_Vulkan_LoadLibrary(nullptr))
//         {
//             throw SDLException("SDL_Vulkan_LoadLibrary failed");
//             // SDL_Log("SDL_Vulkan_LoadLibrary failed: %s", SDL_GetError());
//             return;
//         }

//         window.reset(SDL_CreateWindow("Vulkan Window", 800, 600, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN));

//         if (!window)
//         {
//             throw SDLException("SDL_CreateWindow failed");
//             // SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
//             return;
//         }

//         cout << "App constructor called." << endl;
//     }

//     ~App()
//     {
//         window.reset();
//         SDL_Vulkan_UnloadLibrary();
//         SDL_Quit();
//         cout << "App destructor called." << endl;
//     }

//     void init()
//     {
//         cout << "App initialized." << endl;
//     }

//     void Run()
//     {

//         cout << "App is running." << endl;
//         SDL_ShowWindow(window.get());
//         while (running)
//         {
//             SDL_Event event;
//             for (SDL_Event event; SDL_PollEvent(&event);)
//             {
//                 if (event.type == SDL_EVENT_QUIT)
//                     running = false;
//             }
//         }
//     }
// };

// int main()
// {
//     cout << "Hello, World!" << endl;
//     try
//     {

//         App app;
//         app.init();
//         app.Run();
//     }
//     catch (const SDLException &e)
//     {
//         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL Exception: %s", e.what());
//         SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Exception", e.what(), nullptr);
//         return EXIT_FAILURE;
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "Standard exception: " << e.what() << std::endl;
//         return EXIT_FAILURE;
//     }
//     catch (...)
//     {
//         std::cerr << "Unknown exception occurred." << std::endl;
//         return EXIT_FAILURE;
//     }

//     return 0;
// }

#include <iostream>
#include <SDL3/SDL.h>

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
    SDLApplication(const char *title)
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init failed: %s", SDL_GetError());
        }
        window = SDL_CreateWindow(title, 800, 600, SDL_WINDOW_RESIZABLE);
        // renderer = SDL_CreateRenderer(window, nullptr);
        renderer = SDL_CreateRenderer(window, "opengl");

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

        // surface = SDL_LoadBMP("./assets/test.bmp");
        // if (surface == nullptr)
        // {
        //     SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_LoadBMP failed: %s", SDL_GetError());
        // }
    }

    ~SDLApplication()
    {
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