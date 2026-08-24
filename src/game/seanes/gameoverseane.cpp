#include "game/seanes/gameoverseane.hpp"
#include "game/seanes/playseane.hpp"
#include <fmt/format.h>

GameOverSeane::GameOverSeane(AssetsManager *assetsManager, SDL_Renderer *renderer, int score) : Seane(assetsManager, renderer), finalScore(score)
{
    assetsManager->LoadFont("ui_default", "./assets/font/candy.otf", 24);

    auto font = assetsManager->GetFont("ui_default");

    // Title
    titleEntity = CreateText(renderer, world, font.get(), fmt::format("GAME OVER - Score: {}", finalScore), WINDOW_WIDTH / 2, 100);
    TextElement title = titleEntity.get<TextElement>();
    titleEntity.set<TextElement>({title.texture,
                                  SDL_FRect{title.rect.x - title.rect.w / 2, title.rect.y - title.rect.h / 2, title.rect.w, title.rect.h}});

    // Restart button
    restartButtonEntity = CreateText(renderer, world, font.get(), "Restart", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    TextElement restart = restartButtonEntity.get<TextElement>();
    restartButtonEntity.set<TextElement>({restart.texture,
                                          SDL_FRect{restart.rect.x - restart.rect.w / 2, restart.rect.y - restart.rect.h / 2, restart.rect.w, restart.rect.h}});

    // Main Menu button
    mainMenuButtonEntity = CreateText(renderer, world, font.get(), "Main Menu", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 50);
    TextElement mainMenu = mainMenuButtonEntity.get<TextElement>();
    mainMenuButtonEntity.set<TextElement>({mainMenu.texture,
                                           SDL_FRect{mainMenu.rect.x - mainMenu.rect.w / 2, mainMenu.rect.y - mainMenu.rect.h / 2, mainMenu.rect.w, mainMenu.rect.h}});

    // Quit button
    quitButtonEntity = CreateText(renderer, world, font.get(), "Quit", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 100);
    TextElement quit = quitButtonEntity.get<TextElement>();
    quitButtonEntity.set<TextElement>({quit.texture,
                                       SDL_FRect{quit.rect.x - quit.rect.w / 2, quit.rect.y - quit.rect.h / 2, quit.rect.w, quit.rect.h}});
}

GameOverSeane::~GameOverSeane()
{
}

void GameOverSeane::Render()
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

void GameOverSeane::Update(float deltaTime)
{
}

void GameOverSeane::HandleEvents(SDL_Event *event)
{
    TextElement restart = restartButtonEntity.get<TextElement>();
    TextElement mainMenu = mainMenuButtonEntity.get<TextElement>();
    TextElement quit = quitButtonEntity.get<TextElement>();

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT)
    {
        // Only process if this is a new press (state transition from not pressed to pressed)
        if (!prevMouseButtonState)
        {
            prevMouseButtonState = true;
            int mouseX = event->button.x;
            int mouseY = event->button.y;

            // Check Restart button
            if (mouseX >= restart.rect.x && mouseX <= restart.rect.x + restart.rect.w &&
                mouseY >= restart.rect.y && mouseY <= restart.rect.y + restart.rect.h)
            {
                auto manager = GetSeaneManager();
                if (manager)
                {
                    // Get the PlaySeane instance and reset it
                    Seane *playSeane = manager->GetSeane("PlaySeane");
                    if (playSeane)
                    {
                        // Cast to PlaySeane and call Reset
                        PlaySeane *playSeaneCasted = dynamic_cast<PlaySeane*>(playSeane);
                        if (playSeaneCasted)
                        {
                            playSeaneCasted->Reset();  // Reset game state
                        }
                    }
                    manager->PopSeane();             // Remove GameOverSeane
                    manager->PushSeane("PlaySeane"); // Start new game
                }
            }
            // Check Main Menu button
            else if (mouseX >= mainMenu.rect.x && mouseX <= mainMenu.rect.x + mainMenu.rect.w &&
                     mouseY >= mainMenu.rect.y && mouseY <= mainMenu.rect.y + mainMenu.rect.h)
            {
                auto manager = GetSeaneManager();
                if (manager)
                {
                    manager->PopSeane();             // Remove GameOverSeane
                    manager->PushSeane("ManuSeane"); // Go to main menu
                }
            }
            // Check Quit button
            else if (mouseX >= quit.rect.x && mouseX <= quit.rect.x + quit.rect.w &&
                     mouseY >= quit.rect.y && mouseY <= quit.rect.y + quit.rect.h)
            {
                SDL_Event quitEvent;
                quitEvent.type = SDL_EVENT_QUIT;
                SDL_PushEvent(&quitEvent);
            }
        }
    }
    else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT)
    {
        prevMouseButtonState = false; // Track that button is released
    }
}
