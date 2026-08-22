#include "engine/element/image_element.hpp"

flecs::entity CreateImage(SDL_Renderer *renderer, SDL_Texture *texture, flecs::world &world)
{
    auto entity = world.entity();
    entity.set<ImageElement>({SDL_FRect{50, 50, static_cast<float>(texture->w), static_cast<float>(texture->h)}, texture});
    return entity;
}