#define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL 0
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <format>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_raii.hpp>
using namespace std;

class SDLException final : public std::runtime_error
{
public:
    SDLException(const std::string &message) : std::runtime_error(std::format("SDL Error: {} -> {}", message, SDL_GetError()))
    {
    }
};

class App
{
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window{nullptr, SDL_DestroyWindow};
    bool running{true};

public:
    App()
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            throw SDLException("SDL_Init failed");
            // SDL_Log("SDL_Init failed: %s", SDL_GetError());
            return;
        }

        if (!SDL_Vulkan_LoadLibrary(nullptr))
        {
            throw SDLException("SDL_Vulkan_LoadLibrary failed");
            // SDL_Log("SDL_Vulkan_LoadLibrary failed: %s", SDL_GetError());
            return;
        }

        window.reset(SDL_CreateWindow("Vulkan Window", 800, 600, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN));

        if (!window)
        {
            throw SDLException("SDL_CreateWindow failed");
            // SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
            return;
        }

        cout << "App constructor called." << endl;
    }

    ~App()
    {
        window.reset();
        SDL_Vulkan_UnloadLibrary();
        SDL_Quit();
        cout << "App destructor called." << endl;
    }

    void init()
    {
        cout << "App initialized." << endl;
    }

    void Run()
    {

        cout << "App is running." << endl;
        SDL_ShowWindow(window.get());
        while (running)
        {
            SDL_Event event;
            for (SDL_Event event; SDL_PollEvent(&event);)
            {
                if (event.type == SDL_EVENT_QUIT)
                    running = false;
            }
        }
    }
};

int main()
{
    cout << "Hello, World!" << endl;
    try
    {

        App app;
        app.init();
        app.Run();
    }
    catch (const SDLException &e)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL Exception: %s", e.what());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Exception", e.what(), nullptr);
        return EXIT_FAILURE;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Standard exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Unknown exception occurred." << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
