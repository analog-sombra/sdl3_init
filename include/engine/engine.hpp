#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__
#include <SDL3/SDL.h>
#include <iostream>
#include "const.hpp"
#include <SDL3_ttf/SDL_ttf.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <flecs.h>
#include "game/testseane.hpp"
#include "engine/debug/engine_debug.hpp"
#include "engine/assets_manager.hpp"
#include "engine/seane/seane_manager.hpp"
#include "engine/element/text_element.hpp"
#include "engine/element/image_element.hpp"

class Engine
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    EngineDebug *engineDebug;
    AssetsManager *assetsManager;
    SeaneManager *seaneManager;
    bool running = true;
    bool fullscreen = false;

public:
    Engine(std::string title);
    ~Engine();
    void Run();

    void Render();
    void Update();
    void HandleEvents();
    
    // Seane management
    SeaneManager *GetSeaneManager() const;

    // driver fuctions
    void GetAllRenderDrivers();
    void GetCurrentRenderDriver();
    void SetRenderDriver(std::string driverName);
};

#endif // __ENGINE_HPP__