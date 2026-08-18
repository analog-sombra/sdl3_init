#ifndef __SPRITE_HPP__
#define __SPRITE_HPP__

#include <SDL3/SDL.h>
#include <entt/entt.hpp>

struct Transform
{
    SDL_FRect rect;
    SDL_Texture *texture;
};

void CreateSprite(SDL_Renderer *renderer, entt::registry &registry);
void RenderSprites(SDL_Renderer *renderer, entt::registry &registry);
void DestroySprites(entt::registry &registry);

#endif // __SPRITE_HPP__