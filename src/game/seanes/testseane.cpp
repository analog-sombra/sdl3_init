#include "game/seanes/testseane.hpp"

TestSeane::TestSeane(AssetsManager *assetsManager, SDL_Renderer *renderer) : Seane(assetsManager, renderer)
{
    assetsManager->LoadFont("ui_default", "./assets/font/candy.otf", 24);
    assetsManager->LoadTexture("player_idle", "./assets/images/player.png");
    assetsManager->LoadTexture("ground", "./assets/images/ground.png");

    // basic text
    auto font = assetsManager->GetFont("ui_default");
    CreateText(renderer, world, font.get(), "Hello SDL3!");

    auto texture = assetsManager->GetTexture("player_idle");
    CreateImage(renderer, texture.get(), world);

    CreateRect(world, SDL_FRect{100.f, 100.f, 50.f, 50.f});

    textQuery = world.query<TextElement>();
    imageQuery = world.query<ImageElement>();

    // Create input system
    const float speed = 5.0f;
    inputSystem = world.system<ImageElement>()
                      .each([this, speed](flecs::entity e, ImageElement &imageelement)
                            {
                              auto ctx = static_cast<SDL_Event *>(world.get_ctx());


            // This lambda is called for each entity with ImageElement
            if (ctx->key.key == SDLK_DOWN) {
                imageelement.rect.y += speed;
                SDL_Log("Down key pressed");
            }
            else if (ctx->key.key == SDLK_UP) {
                imageelement.rect.y -= speed;
                SDL_Log("Up key pressed");
            }
            else if (ctx->key.key == SDLK_LEFT) {
                imageelement.rect.x -= speed;
                SDL_Log("Left key pressed");
            }
            else if (ctx->key.key == SDLK_RIGHT) {
                imageelement.rect.x += speed;
                SDL_Log("Right key pressed");
            } });
}

TestSeane::~TestSeane()
{
}

void TestSeane::HandleEvents(SDL_Event *event)
{
    world.set_ctx(event);
    inputSystem.run();
}

void TestSeane::Render()
{

    auto q = textQuery;

    q.each([this](flecs::entity e, TextElement &textelement)
           {
            SDL_FRect rect{
                textelement.rect.x,
                textelement.rect.y,
                textelement.rect.w,
                textelement.rect.h
            };
            SDL_RenderTexture(this->renderer, textelement.texture, nullptr, &rect); });

    auto q2 = imageQuery;

    q2.each([this](flecs::entity e, ImageElement &imageelement)
            {
        SDL_FRect rect{
            imageelement.rect.x,
            imageelement.rect.y,
            imageelement.rect.w,
            imageelement.rect.h
        };
        SDL_RenderTexture(this->renderer, imageelement.texture, nullptr, &rect); });

    auto q3 = world.query<RectElement>();
    q3.each([this](flecs::entity e, RectElement &rectElement)
            {  
             SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255); // Red color
             SDL_RenderFillRect(this->renderer, &rectElement.rect); });
}

void TestSeane::Update()
{
}
