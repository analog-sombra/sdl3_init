#ifndef __PAUSE_SEANE_HPP__
#define __PAUSE_SEANE_HPP__

#include <SDL3/SDL.h>
#include "engine/assets_manager.hpp"
#include "engine/seane/seane.hpp"
#include "const.hpp"
#include "engine/seane/seane_manager.hpp"
#include <flecs.h>
#include "engine/element/text_element.hpp"
#include <iostream>

class PauseSeane : public Seane
{

    flecs::entity titleEntity;
    flecs::entity resumeButtonEntity;
    flecs::entity restartButtonEntity;
    flecs::entity mainMenuButtonEntity;
    flecs::entity exitButtonEntity;
    bool prevMouseButtonState = false;  // Track previous frame's mouse state

public:
    PauseSeane(AssetsManager *assetsManager, SDL_Renderer *renderer);
    ~PauseSeane();
    void Render();
    void Update(float deltaTime); // Now accepts deltaTime parameter
    void HandleEvents(SDL_Event *event);
};

#endif // __PAUSE_SEANE_HPP__