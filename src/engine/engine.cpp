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
    renderer = SDL_CreateRenderer(window, nullptr);

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

    // Initialize AssetsManager
    assetsManager = new AssetsManager(renderer);

    // Initialize SeaneManager with shared_ptr
    seaneManager = std::make_shared<SeaneManager>(assetsManager, renderer);

    // Initialize all game seanes using GameSetup
    // NOTE: All seane setup logic is now in game/setup/game_setup.cpp
    // To add new seanes, modify GameSetup::InitializeSeanes() in game/setup/
    GameSetup::InitializeSeanes(seaneManager, assetsManager, renderer);

    SDL_Log("Engine initialized successfully");
}

Engine::~Engine()
{
    // Clean up SeaneManager (shared_ptr auto cleanup)
    seaneManager.reset();
    // Clean up EngineDebug
    delete engineDebug;
    // Clean up AssetsManager
    delete assetsManager;

    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    TTF_Quit();

    // Quit SDL
    SDL_Quit();
}

void Engine::Run()
{
    Uint64 fps = 0;
    Uint64 fpsTimer = SDL_GetTicks();
    lastFrameTime = SDL_GetTicks();
    
    while (running)
    {
        Uint64 currentFrameTime = SDL_GetTicks();
        deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f;  // Convert to seconds
        lastFrameTime = currentFrameTime;
        
        // Cap deltaTime to prevent large jumps (e.g., if paused/debugged)
        if (deltaTime > 0.05f)  // Max 50ms per frame
            deltaTime = 0.05f;
        
        HandleEvents();
        Update();
        Render();
        fps++;

        Uint64 now = SDL_GetTicks();

        if (now - fpsTimer >= 1000)
        {
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
    seaneManager->Render();

    SDL_RenderPresent(renderer);
}

void Engine::Update()
{
    seaneManager->Update(deltaTime);
}

void Engine::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        engineDebug->ProcessEvent(&event);
        seaneManager->HandleEvents(&event);
        if (event.type == SDL_EVENT_QUIT)
        {
            running = false;
        }
        if (event.key.key == SDLK_F11)
        {
            SDL_SetWindowFullscreen(window, fullscreen);
            fullscreen = !fullscreen;
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