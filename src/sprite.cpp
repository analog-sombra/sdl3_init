#include "sprite.hpp"

void CreateSprite(SDL_Renderer *renderer, entt::registry &registry)
{
    auto entity = registry.create();
    SDL_Surface *surface = SDL_LoadBMP("./assets/test.bmp");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    registry.emplace<Transform>(
        entity,
        SDL_FRect{0.0f, 0.0f, 50, 50},
        texture);
    registry.emplace<Player>(entity);
    SDL_DestroySurface(surface);

    // for (int i = 0; i < 10; i++)
    // {
    //     auto entity = registry.create();

    //     SDL_Surface *surface = SDL_LoadBMP("./assets/test.bmp");
    //     SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    //     registry.emplace<Transform>(
    //         entity,
    //         SDL_FRect{50.0f + i * 70.0f, 50.0f, 50, 50},
    //         texture);
    //     SDL_DestroySurface(surface);
    // }
}

void RenderSprites(SDL_Renderer *renderer, entt::registry &registry)
{
    auto view = registry.view<Transform>();

    for (auto entity : view)
    {
        auto &transform = view.get<Transform>(entity);

        SDL_FRect rect{
            transform.rect.x,
            transform.rect.y,
            transform.rect.w,
            transform.rect.h};

        SDL_RenderTexture(renderer, transform.texture, nullptr, &rect);
    }
}

void DestroySprites(entt::registry &registry)
{
    auto view = registry.view<Transform>();

    for (auto entity : view)
    {
        auto &transform = view.get<Transform>(entity);
        if (transform.texture)
            SDL_DestroyTexture(transform.texture);
    }
    registry.clear<Transform>();
}