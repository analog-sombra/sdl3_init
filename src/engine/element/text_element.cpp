#include "engine/element/text_element.hpp"
flecs::entity CreateText(SDL_Renderer *renderer, flecs::world &world, TTF_Font *font, const std::string &text)
{
    auto entity = world.entity();

    SDL_Surface *textSurface = TTF_RenderText_Blended(font, text.c_str(), 0, SDL_Color{255, 255, 255, 255});
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    entity.set<TextElement>(
        {textTexture,
         SDL_FRect{10.f, 50.f, 200.f, 50.f}});
    SDL_DestroySurface(textSurface);
    return entity;
}
