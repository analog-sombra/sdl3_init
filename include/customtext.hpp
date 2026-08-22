#ifndef __CUSTOMTEXT_HPP__
#define __CUSTOMTEXT_HPP__

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <flecs.h>


struct TextElement
{
    TTF_Font *font;
    SDL_Texture *texture;
    SDL_FRect rect;
};

void CreateText(SDL_Renderer *renderer, flecs::world &world, const std::string &text);
void RenderText(SDL_Renderer *renderer, flecs::world &world);
void DestroyText(flecs::world &world);

#endif // __CUSTOMTEXT_HPP__