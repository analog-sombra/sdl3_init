#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__
#include <SDL3/SDL.h>
#include <iostream>
#include "const.hpp"
#include <SDL3_ttf/SDL_ttf.h>
#include <fmt/core.h>
#include "engine/debug/engine_debug.hpp"
#include "engine/assets_manager.hpp"

class Engine
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running = true;
    EngineDebug *engineDebug;
    AssetsManager *assetsManager;

public:
    Engine(std::string title);
    ~Engine();
    void Run();

    void Render();
    void Update();
    void HandleEvents();

    // driver fuctions
    void GetAllRenderDrivers();
    void GetCurrentRenderDriver();
    void SetRenderDriver(std::string driverName);
};

#endif // __ENGINE_HPP__