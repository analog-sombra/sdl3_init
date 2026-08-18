
#include "customtext.hpp"

void CreateText(SDL_Renderer *renderer, entt::registry &registry, const std::string &text)
{
    auto entity = registry.create();

    TTF_Font *font = TTF_OpenFont("./assets/font/candy.otf", 24.f);
    if (font == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF_OpenFont failed: %s", SDL_GetError());
    }
    // TTF_SetFontSize(font, 200);
    // TTF_SetFontOutline(font, 1);
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, text.c_str(), 0, SDL_Color{255, 255, 255, 255});
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    registry.emplace<CustomText>(
        entity,
        font,
        textTexture,
        SDL_FRect{10.f, 50.f, 200.f, 50.f});
    SDL_DestroySurface(textSurface);
}

void RenderText(SDL_Renderer *renderer, entt::registry &registry)
{
    auto view = registry.view<CustomText>();

    for (auto entity : view)
    {
        auto &customtext = view.get<CustomText>(entity);

        SDL_FRect rect{
            customtext.rect.x,
            customtext.rect.y,
            customtext.rect.w,
            customtext.rect.h};

        SDL_RenderTexture(renderer, customtext.texture, nullptr, &rect);
    }
}

void DestroyText(entt::registry &registry)
{
    auto view = registry.view<CustomText>();

    for (auto entity : view)
    {
        auto &customtext = view.get<CustomText>(entity);
        if (customtext.texture)
            SDL_DestroyTexture(customtext.texture);
        if (customtext.font)
            TTF_CloseFont(customtext.font);
    }
}
