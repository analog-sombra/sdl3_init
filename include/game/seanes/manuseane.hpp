#ifndef __MANUS_SEANE_HPP__
#define __MANUS_SEANE_HPP__

#include <SDL3/SDL.h>
#include "engine/assets_manager.hpp"
#include "engine/seane/seane.hpp"
#include "const.hpp"
#include "engine/seane/seane_manager.hpp"
#include <flecs.h>
#include "engine/element/text_element.hpp"
#include <iostream>

class ManuSeane : public Seane
{
    flecs::entity playButtonEntity;
    flecs::entity exitButtonEntity;
    flecs::entity titleButtonEntity;
    bool clickProcessed = false;  // Debounce flag

public:
    ManuSeane(AssetsManager *assetsManager, SDL_Renderer *renderer);
    ~ManuSeane();
    void Render();
    void Update(float deltaTime); // Now accepts deltaTime parameter
    void HandleEvents(SDL_Event *event);
};

#endif // __MANUS_SEANE_HPP__