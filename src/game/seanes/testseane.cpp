#include "game/seanes/testseane.hpp"

TestSeane::TestSeane(AssetsManager *assetsManager, SDL_Renderer *renderer) : Seane(assetsManager, renderer)
{
    assetsManager->LoadFont("ui_default", "./assets/font/candy.otf", 24);
    assetsManager->LoadTexture("player_idle", "./assets/images/player.png");
    assetsManager->LoadTexture("ground", "./assets/images/ground.png");

    // basic text
    auto font = assetsManager->GetFont("ui_default");
    CreateText(renderer, world, font.get(), "Bubble Sort Visualization", 10, 10);

    auto texture = assetsManager->GetTexture("player_idle");
    CreateImage(renderer, texture.get(), world, 10, 10);

    const float boxs = 20.f;
    gap = 10.f;
    padding = 10.f;
    width = (WINDOW_WIDTH - padding * 2 - gap * (boxs - 1)) / boxs;

    const float min_h = 20.f;
    const float max_h = WINDOW_HEIGHT - 20.f;
    // 20 random heights from min_h to max_h

    for (int i = 0; i < boxs; i++)
    {
        float random_h = min_h + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max_h - min_h)));
        hight[i] = random_h;
    }

    for (int i = 0; i < boxs; i++)
    {
        SDL_FRect rect{padding + i * (width + gap), WINDOW_HEIGHT - hight[i], width, hight[i]};
        rectEntities.push_back(CreateRect(world, rect));
    }

    sorting = true; // Start sorting

    // CreateRect(world, SDL_FRect{100.f, 100.f, 50.f, 50.f});

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

    // auto q = textQuery;

    // q.each([this](flecs::entity e, TextElement &textelement)
    //        {
    //         SDL_FRect rect{
    //             textelement.rect.x,
    //             textelement.rect.y,
    //             textelement.rect.w,
    //             textelement.rect.h
    //         };
    //         SDL_RenderTexture(this->renderer, textelement.texture, nullptr, &rect); });

    // auto q2 = imageQuery;

    // q2.each([this](flecs::entity e, ImageElement &imageelement)
    //         {
    //     SDL_FRect rect{
    //         imageelement.rect.x,
    //         imageelement.rect.y,
    //         imageelement.rect.w,
    //         imageelement.rect.h
    //     };
    //     SDL_RenderTexture(this->renderer, imageelement.texture, nullptr, &rect); });

    auto q3 = world.query<RectElement>();
    int index = 0;
    q3.each([this, &index](flecs::entity e, RectElement &rectElement)
            {  
             // Color based on sorting state
             if (sorting && (index == sortJ || index == sortJ + 1))
             {
                 SDL_SetRenderDrawColor(this->renderer, 255, 255, 0, 255); // Yellow for comparing
             }
             else if (index >= 20 - sortI)
             {
                 SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255); // Green for sorted
             }
             else
             {
                 SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255); // Red for unsorted
             }
             SDL_RenderFillRect(this->renderer, &rectElement.rect);
             index++; });
}

void TestSeane::Update(float deltaTime)
{
    // Update timers
    timerManager.Update(deltaTime);

    // Bubble sort one step per stepDuration
    if (!sorting)
        return;

    elapsedTime += deltaTime;
    if (elapsedTime < stepDuration)
        return; // Not enough time has passed yet

    elapsedTime = 0.0f; // Reset timer

    const int n = 20;

    if (sortI < n - 1)
    {
        if (sortJ < n - sortI - 1)
        {
            if (hight[sortJ] > hight[sortJ + 1])
            {
                // Swap heights
                std::swap(hight[sortJ], hight[sortJ + 1]);

                // Update rect positions
                UpdateRectPosition(sortJ, hight[sortJ]);
                UpdateRectPosition(sortJ + 1, hight[sortJ + 1]);
            }
            sortJ++;
        }
        else
        {
            sortJ = 0;
            sortI++;
        }
    }
    else
    {
        sorting = false; // Sorting complete
        SDL_Log("Bubble sort complete!");
    }
}

void TestSeane::UpdateRectPosition(int index, float newHeight)
{
    if (index < 0 || index >= rectEntities.size())
        return;

    auto entity = rectEntities[index];
    RectElement &rectElement = entity.get_mut<RectElement>();

    rectElement.rect.y = WINDOW_HEIGHT - newHeight;
    rectElement.rect.h = newHeight;
}
