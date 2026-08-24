#ifndef __SPLASH_SEANE_HPP__
#define __SPLASH_SEANE_HPP__

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <flecs.h>
#include <string>
#include <map>
#include <memory>
#include "engine/element/component.hpp"
#include "engine/assets_manager.hpp"
#include "engine/element/text_element.hpp"
#include "engine/element/image_element.hpp"
#include "engine/seane/seane.hpp"
#include "engine/element/rect_element.hpp"
#include "engine/timer/timer.hpp"
#include "const.hpp"
#include <random>
#include <vector>
#include "engine/seane/seane_manager.hpp"

class SplashSeane : public Seane
{
    // Timer manager for scheduling
    TimerManager timerManager;

public:
    SplashSeane(AssetsManager *assetsManager, SDL_Renderer *renderer);
    ~SplashSeane();
    void Render();
    void Update(float deltaTime); // Now accepts deltaTime parameter
    void HandleEvents(SDL_Event *event);
};

#endif // __SPLASH_SEANE_HPP__