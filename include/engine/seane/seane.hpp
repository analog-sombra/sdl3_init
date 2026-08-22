#ifndef __SEANE_HPP__
#define __SEANE_HPP__

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <flecs.h>
#include <string>
#include <map>
#include <memory>
#include "../element/component.hpp"
#include "../assets_manager.hpp"
#include "../element/text_element.hpp"
#include "../element/image_element.hpp"

class Seane
{
protected:
    flecs::world world;
    AssetsManager *assetsManager;
    SDL_Renderer *renderer;

public:
    Seane(AssetsManager *assetsManager, SDL_Renderer *renderer) 
        : world(), assetsManager(assetsManager), renderer(renderer) {};
    virtual ~Seane() {};
    virtual void Render() = 0;
    virtual void Update() = 0;
    virtual void HandleEvents(SDL_Event *event) = 0;
};

#endif // __SEANE_HPP__