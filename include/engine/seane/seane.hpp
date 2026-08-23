#ifndef __SEANE_HPP__
#define __SEANE_HPP__

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

class SeaneManager; // Forward declaration

class Seane
{
protected:
    flecs::world world;
    AssetsManager *assetsManager;
    SDL_Renderer *renderer;
    std::weak_ptr<SeaneManager> seaneManager;

public:
    Seane(AssetsManager *assetsManager, SDL_Renderer *renderer) 
        : world(), assetsManager(assetsManager), renderer(renderer) {};
    virtual ~Seane() {};
    
    void SetSeaneManager(std::weak_ptr<SeaneManager> manager) {
        seaneManager = manager;
    }
    
    std::shared_ptr<SeaneManager> GetSeaneManager() const {
        return seaneManager.lock();
    }
    
    virtual void Render() = 0;
    virtual void Update() = 0;
    virtual void HandleEvents(SDL_Event *event) = 0;
};

#endif // __SEANE_HPP__