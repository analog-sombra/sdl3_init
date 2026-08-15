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
struct SDLApplication
{
    SDL_Window *window;
    SDL_Surface *surface;
    bool running = true;
    SDLApplication(const char *title)
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init failed: %s", SDL_GetError());
        }
        window = SDL_CreateWindow(title, 800, 600, SDL_WINDOW_RESIZABLE);
        surface = SDL_LoadBMP("./assets/test.bmp");
        if (surface == nullptr)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_LoadBMP failed: %s", SDL_GetError());
        }
    }

    ~SDLApplication()
    {
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
        SDL_Surface *windowSurface = SDL_GetWindowSurface(window);
        if (nullptr != windowSurface)
        {

            SDL_BlitSurface(surface, nullptr, windowSurface, nullptr);
            SDL_UpdateWindowSurface(window);
        }
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