#ifndef __TEST_SEANE_HPP__
#define __TEST_SEANE_HPP__

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

class TestSeane : public Seane
{
    flecs::system inputSystem;
    flecs::query<TextElement> textQuery;
    flecs::query<ImageElement> imageQuery;

    // Timer manager for scheduling
    TimerManager timerManager;

    float hight[20];
    std::vector<flecs::entity> rectEntities;  // Store rect entities for updating
    
    // Bubble sort state
    int sortI = 0;
    int sortJ = 0;
    bool sorting = false;
    float padding = 10.f;
    float width = 0.f;
    float gap = 10.f;
    
    // Timing with deltaTime
    const float stepDuration = 0.3f;  // Time between sort steps in seconds
    float elapsedTime = 0.0f;

public:
    TestSeane(AssetsManager *assetsManager, SDL_Renderer *renderer);
    ~TestSeane();
    void Render();
    void Update(float deltaTime);  // Now accepts deltaTime parameter
    void UpdateRectPosition(int index, float newHeight);
    void HandleEvents(SDL_Event *event);
};

#endif // __TEST_SEANE_HPP__