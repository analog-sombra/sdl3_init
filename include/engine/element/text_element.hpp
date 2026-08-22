#ifndef _TEXT_ELEMENT_HPP__
#define _TEXT_ELEMENT_HPP__

#include "component.hpp"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <flecs.h>

flecs::entity CreateText(SDL_Renderer *renderer, flecs::world &world, TTF_Font *font, const std::string &text);

#endif // _TEXT_ELEMENT_HPP__