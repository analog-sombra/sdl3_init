#include "game/seanes/manuseane.hpp"

ManuSeane::ManuSeane(AssetsManager *assetsManager, SDL_Renderer *renderer) : Seane(assetsManager, renderer)
{
    assetsManager->LoadFont("ui_default", "./assets/font/candy.otf", 24);
    assetsManager->LoadFont("ui_small", "./assets/font/candy.otf", 12);

    // basic text
    auto font = assetsManager->GetFont("ui_default");
    auto font_small = assetsManager->GetFont("ui_small");
    titleButtonEntity = CreateText(renderer, world, font.get(), "Snake Will Eat You", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    TextElement title = titleButtonEntity.get<TextElement>();
    titleButtonEntity.set<TextElement>({title.texture,
                                        SDL_FRect{title.rect.x - title.rect.w / 2, title.rect.y - title.rect.h / 2, title.rect.w, title.rect.h}});
    playButtonEntity = CreateText(renderer, world, font.get(), "Play", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 50);

    TextElement play = playButtonEntity.get<TextElement>();
    playButtonEntity.set<TextElement>({play.texture,
                                       SDL_FRect{play.rect.x - play.rect.w / 2, play.rect.y - play.rect.h / 2, play.rect.w, play.rect.h}});
    exitButtonEntity = CreateText(renderer, world, font.get(), "Exit", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 100);
    TextElement exit = exitButtonEntity.get<TextElement>();
    exitButtonEntity.set<TextElement>({exit.texture,
                                       SDL_FRect{exit.rect.x - exit.rect.w / 2, exit.rect.y - exit.rect.h / 2, exit.rect.w, exit.rect.h}});

    CreateText(renderer, world, font_small.get(), "github.com/analog-sombra", WINDOW_WIDTH - 600, WINDOW_HEIGHT - 30);
}

ManuSeane::~ManuSeane()
{
}

void ManuSeane::Render()
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

void ManuSeane::Update(float deltaTime)
{
}
void ManuSeane::HandleEvents(SDL_Event *event)
{
    TextElement exit = exitButtonEntity.get<TextElement>();
    TextElement play = playButtonEntity.get<TextElement>();
    
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT && !clickProcessed)
    {
        int mouseX = event->button.x;
        int mouseY = event->button.y;
        
        clickProcessed = true;  // Set flag to prevent multiple processing

        // Check Exit button
        if (mouseX >= exit.rect.x && mouseX <= exit.rect.x + exit.rect.w &&
            mouseY >= exit.rect.y && mouseY <= exit.rect.y + exit.rect.h)
        {
            SDL_Log("Exit button clicked");
            // Handle exit button click, e.g., set a flag to quit the game
            if (auto manager = seaneManager.lock())
            {
                manager->PopSeane(); // Remove the current ManuSeane
                SDL_Event quitEvent;
                quitEvent.type = SDL_EVENT_QUIT;
                SDL_PushEvent(&quitEvent);
            }
        }
        // Check Play button
        else if (mouseX >= play.rect.x && mouseX <= play.rect.x + play.rect.w &&
                 mouseY >= play.rect.y && mouseY <= play.rect.y + play.rect.h)
        {
            SDL_Log("Play button clicked");
            // Handle play button click, e.g., transition to the next seane
            if (auto manager = seaneManager.lock())
            {
                manager->PopSeane();             // Remove the current ManuSeane
                manager->PushSeane("PlaySeane"); // Transition to PlaySeane
            }
        }
    }
    else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT)
    {
        clickProcessed = false;  // Reset flag on mouse up
    }
}