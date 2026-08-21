#include "engine/engine.hpp"

Engine::Engine(std::string title)
{

    // Initialize SDL with video support
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init failed: %s", SDL_GetError());
    }

    // Create an SDL window
    window = SDL_CreateWindow(title.c_str(), WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);

    if (window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow failed: %s", SDL_GetError());
    }

    // Create an SDL renderer
    renderer = SDL_CreateRenderer(window, "opengl");

    if (renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateRenderer failed: %s", SDL_GetError());
    }

    // Initialize SDL_ttf for text rendering
    if (!TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF_Init failed: %s", SDL_GetError());
    }

    // Initialize EngineDebug
    engineDebug = new EngineDebug(window, renderer);
}
Engine::~Engine()
{
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    TTF_Quit();
    delete engineDebug;
    SDL_Quit();
}

void Engine::Run()
{
    Uint64 fps = 0;
    Uint64 fpsTimer = SDL_GetTicks();
    while (running)
    {
        HandleEvents();
        Update();
        Render();
        fps++;

        Uint64 now = SDL_GetTicks();

        if (now - fpsTimer >= 1000)
        {
            // SDL_Log("FPS: %llu", fps);
            SDL_SetWindowTitle(window, fmt::format("Engine Window - FPS: {}", fps).c_str());

            fps = 0;
            fpsTimer = now;
        }
    }
}

void Engine::Render()
{
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    engineDebug->NewFrame();
    engineDebug->Render(renderer);
    SDL_RenderDebugText(renderer, 10, 10, "Hello SDL3!");

    SDL_RenderPresent(renderer);
}
void Engine::Update()
{
}
void Engine::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        engineDebug->ProcessEvent(&event);
        if (event.type == SDL_EVENT_QUIT)
        {
            running = false;
        }
    }
}

// driver functions
void Engine::GetAllRenderDrivers()
{
    int numDrivers = SDL_GetNumRenderDrivers();
    for (int i = 0; i < numDrivers; ++i)
    {
        SDL_Log(fmt::format("Render Driver {}: {}", i, SDL_GetRenderDriver(i)).c_str());
    }
}

void Engine::GetCurrentRenderDriver()
{
    const char *rendererName = SDL_GetRendererName(renderer);
    SDL_Log(fmt::format("Current Render Driver: {}", rendererName).c_str());
}

void Engine::SetRenderDriver(std::string driverName)
{
    // Destroy the current renderer
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    // Create a new renderer with the specified driver name
    renderer = SDL_CreateRenderer(window, driverName.c_str());

    if (renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateRenderer failed: %s", SDL_GetError());
    }
}