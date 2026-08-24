#ifndef __GAMEOVER_SEANE_HPP__
#define __GAMEOVER_SEANE_HPP__

#include <SDL3/SDL.h>
#include "engine/assets_manager.hpp"
#include "engine/seane/seane.hpp"
#include "const.hpp"
#include "engine/seane/seane_manager.hpp"
#include <flecs.h>
#include "engine/element/text_element.hpp"
#include <iostream>

class GameOverSeane : public Seane
{
    flecs::entity titleEntity;
    flecs::entity restartButtonEntity;
    flecs::entity mainMenuButtonEntity;
    flecs::entity quitButtonEntity;
    int finalScore;
    bool prevMouseButtonState = false;  // Track previous frame's mouse state

public:
    GameOverSeane(AssetsManager *assetsManager, SDL_Renderer *renderer, int score);
    ~GameOverSeane();
    void Render();
    void Update(float deltaTime);
    void HandleEvents(SDL_Event *event);
};

#endif // __GAMEOVER_SEANE_HPP__
