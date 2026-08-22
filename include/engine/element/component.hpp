#ifndef __ELEMENT_COMPONENT_HPP__
#define __ELEMENT_COMPONENT_HPP__

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

struct TextElement
{
    SDL_Texture *texture;
    SDL_FRect rect;
};

struct ImageElement
{
    SDL_FRect rect;
    SDL_Texture *texture;
};

#endif // __ELEMENT_COMPONENT_HPP__