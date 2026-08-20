#include "sprite.hpp"



void CreateSprite(SDL_Renderer *renderer, flecs::world &world, std::string path, float x, float y, bool isPlayer, b2BodyId bodyId)
{
    auto entity = world.entity();
    // SDL_Surface *surface = SDL_LoadBMP("./assets/test.bmp");
    // SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Texture *texture = IMG_LoadTexture(renderer, path.c_str());

    entity.set<Transform>({SDL_FRect{x, y, static_cast<float>(texture->w), static_cast<float>(texture->h)}, texture});
    entity.set<PhysicsBody>({bodyId}); // Initialize with the provided body ID
    if (isPlayer)
        entity.add<Player>();
    // SDL_DestroySurface(surface);
}

void RenderSprites(SDL_Renderer *renderer, flecs::world &world)
{
    auto q = world.query_builder<Transform>().build();

    q.each([renderer](flecs::entity e, Transform &transform)
           {

        SDL_FRect rect{
            transform.rect.x,
            transform.rect.y,
            transform.rect.w,
            transform.rect.h};

        SDL_RenderTexture(renderer, transform.texture, nullptr, &rect); });
}

void DestroySprites(flecs::world &world)
{
    // auto view = registry.view<Transform>();
    auto q = world.query_builder<Transform>().build();

    q.each([](flecs::entity e, Transform &transform)
           {
        if (transform.texture) SDL_DestroyTexture(transform.texture); });
}

void SyncPhysicsToTransform(flecs::world &world)
{
    auto query =
        world.query<Transform, PhysicsBody>();

    query.each([](
                   Transform &transform,
                   PhysicsBody &physics)
               {
        b2Vec2 position =
            b2Body_GetPosition(physics.bodyId);

        transform.rect.x =
            MetersToPixels(position.x) -
            transform.rect.w / 2.0f;

        transform.rect.y =
            MetersToPixels(position.y) -
            transform.rect.h / 2.0f; });
}