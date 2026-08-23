#ifndef _RECT_ELEMENT_HPP__
#define _RECT_ELEMENT_HPP__

#include "component.hpp"
#include <iostream>
#include <SDL3/SDL.h>
#include <flecs.h>

flecs::entity CreateRect(flecs::world &world, SDL_FRect rect);

#endif // _RECT_ELEMENT_HPP__