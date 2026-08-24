#include "game/seanes/splashseane.hpp"

SplashSeane::SplashSeane(AssetsManager *assetsManager, SDL_Renderer *renderer) : Seane(assetsManager, renderer)
{
    assetsManager->LoadFont("ui_default", "./assets/font/candy.otf", 24);
    assetsManager->LoadTexture("player_idle", "./assets/images/player.png");
    assetsManager->LoadTexture("ground", "./assets/images/ground.png");

    // basic text
    auto font = assetsManager->GetFont("ui_default");
    CreateText(renderer, world, font.get(), "Snake Will Eat You", WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 50);
    // SDL_Log("SplashSeane initialized");
    // Schedule transition to TestSeane after 5 seconds
    timerManager.Schedule([this]()
    {
        if (auto manager = seaneManager.lock())
        {
            manager->PopSeane(); // Remove the current SplashSeane
            manager->PushSeane("TestSeane");
        }
    }, 5.0f);
}

SplashSeane::~SplashSeane()
{
}

void SplashSeane::Render()
{
    auto q3 = world.query<TextElement>();
    q3.each([this](flecs::entity e, TextElement &textelement)
            {
            SDL_FRect rect{
                textelement.rect.x,
                textelement.rect.y,
                textelement.rect.w,
                textelement.rect.h
            };
            SDL_RenderTexture(this->renderer, textelement.texture, nullptr, &rect); });
}

void SplashSeane::Update(float deltaTime)
{
    // Update timers to execute scheduled callbacks
    timerManager.Update(deltaTime);
}

void SplashSeane::HandleEvents(SDL_Event *event)
{
}