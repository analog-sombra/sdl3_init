#include "engine/element/text_element.hpp"
flecs::entity CreateText(SDL_Renderer *renderer, flecs::world &world, TTF_Font *font, const std::string &text, float x, float y)
{
    auto entity = world.entity();

    SDL_Surface *textSurface = TTF_RenderText_Blended(font, text.c_str(), 0, SDL_Color{255, 255, 255, 255});
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    entity.set<TextElement>(
        {textTexture,
         SDL_FRect{x, y, static_cast<float>(textSurface->w), static_cast<float>(textSurface->h)}});
    SDL_DestroySurface(textSurface);
    return entity;
}
