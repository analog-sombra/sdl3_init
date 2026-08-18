#ifndef __CUSTOMTEXT_HPP__
#define __CUSTOMTEXT_HPP__

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <entt/entt.hpp>

struct CustomText
{
    TTF_Font *font;
    SDL_Texture *texture;
    SDL_FRect rect;
};

void CreateText(SDL_Renderer *renderer, entt::registry &registry, const std::string &text);
void RenderText(SDL_Renderer *renderer, entt::registry &registry);
void DestroyText(entt::registry &registry);

#endif // __CUSTOMTEXT_HPP__