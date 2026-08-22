
#include "customtext.hpp"

void CreateText(SDL_Renderer *renderer, flecs::world &world, const std::string &text)
{
    auto entity = world.entity();

    TTF_Font *font = TTF_OpenFont("./assets/font/candy.otf", 24.f);
    if (font == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF_OpenFont failed: %s", SDL_GetError());
    }
    // TTF_SetFontSize(font, 200);
    // TTF_SetFontOutline(font, 1);
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, text.c_str(), 0, SDL_Color{255, 255, 255, 255});
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    entity.set<TextElement>(
        {font,
         textTexture,
         SDL_FRect{10.f, 50.f, 200.f, 50.f}});
    SDL_DestroySurface(textSurface);
}

void RenderText(SDL_Renderer *renderer, flecs::world &world)
{
    // auto view = registry.view<TextElement>();
    auto q = world.query_builder<TextElement>().build();


    q.each([renderer](flecs::entity e, TextElement &textelement)
    {
        SDL_FRect rect{
            textelement.rect.x,
            textelement.rect.y,
            textelement.rect.w,
            textelement.rect.h};

        SDL_RenderTexture(renderer, textelement.texture, nullptr, &rect);
    });
}

void DestroyText(flecs::world &world)
{

    auto q = world.query_builder<TextElement>().build();

    q.each([](flecs::entity e, TextElement &text)
           {
        if (text.texture) SDL_DestroyTexture(text.texture);
        if (text.font)    TTF_CloseFont(text.font); });
}
