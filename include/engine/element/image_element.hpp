#ifndef __IMAGE_ELEMENT_HPP__
#define __IMAGE_ELEMENT_HPP__

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <flecs.h>
#include <string>
#include "component.hpp"
#include "const.hpp"
flecs::entity CreateImage(
    SDL_Renderer *renderer,
    SDL_Texture *texture,
    flecs::world &world,
    float x, float y);

#endif // __IMAGE_ELEMENT_HPP__