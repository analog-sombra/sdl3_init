#ifndef __SPRITE_HPP__
#define __SPRITE_HPP__

#include <SDL3/SDL.h>
#include <flecs.h>
#include <SDL3_image/SDL_image.h>
#include "const.hpp"
#include <iostream>
#include <box2d/box2d.h>

struct PhysicsBody
{
    b2BodyId bodyId = b2_nullBodyId;
};

struct Transform
{
    SDL_FRect rect;
    SDL_Texture *texture;
};

struct Player
{
};

void CreateSprite(
    SDL_Renderer *renderer,
    flecs::world &world,
    std::string path,
    float x,
    float y,
    bool isPlayer,
    b2BodyId bodyId);
void RenderSprites(SDL_Renderer *renderer, flecs::world &world);
void DestroySprites(flecs::world &world);
void SyncPhysicsToTransform(flecs::world &world);

#endif // __SPRITE_HPP__