#include "engine/element/rect_element.hpp"
flecs::entity CreateRect(flecs::world &world, SDL_FRect rect)
{
    auto entity = world.entity();
    entity.set<RectElement>({rect});
    return entity;
}
