#include "game/seanes/pauseseane.hpp"
#include "game/seanes/playseane.hpp"

PauseSeane::PauseSeane(AssetsManager *assetsManager, SDL_Renderer *renderer) : Seane(assetsManager, renderer)
{
    assetsManager->LoadFont("ui_default", "./assets/font/candy.otf", 24);
    assetsManager->LoadFont("ui_small", "./assets/font/candy.otf", 12);

    // basic text
    auto font = assetsManager->GetFont("ui_default");
    auto font_small = assetsManager->GetFont("ui_small");

    titleEntity = CreateText(renderer, world, font.get(), "Paused", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50);

    TextElement title = titleEntity.get<TextElement>();
    titleEntity.set<TextElement>({title.texture,
                                  SDL_FRect{title.rect.x - title.rect.w / 2, title.rect.y - title.rect.h / 2, title.rect.w, title.rect.h}});
    resumeButtonEntity = CreateText(renderer, world, font.get(), "Resume", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    TextElement resume = resumeButtonEntity.get<TextElement>();
    resumeButtonEntity.set<TextElement>({resume.texture,
                                         SDL_FRect{resume.rect.x - resume.rect.w / 2, resume.rect.y - resume.rect.h / 2, resume.rect.w, resume.rect.h}});

    restartButtonEntity = CreateText(renderer, world, font.get(), "Restart", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 25);

    TextElement restart = restartButtonEntity.get<TextElement>();
    restartButtonEntity.set<TextElement>({restart.texture,
                                          SDL_FRect{restart.rect.x - restart.rect.w / 2, restart.rect.y - restart.rect.h / 2, restart.rect.w, restart.rect.h}});

    mainMenuButtonEntity = CreateText(renderer, world, font.get(), "Main Menu", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 50);

    TextElement mainMenu = mainMenuButtonEntity.get<TextElement>();
    mainMenuButtonEntity.set<TextElement>({mainMenu.texture,
                                           SDL_FRect{mainMenu.rect.x - mainMenu.rect.w / 2, mainMenu.rect.y - mainMenu.rect.h / 2, mainMenu.rect.w, mainMenu.rect.h}});

    exitButtonEntity = CreateText(renderer, world, font.get(), "Exit", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 75);

    TextElement exit = exitButtonEntity.get<TextElement>();
    exitButtonEntity.set<TextElement>({exit.texture,
                                       SDL_FRect{exit.rect.x - exit.rect.w / 2, exit.rect.y - exit.rect.h / 2, exit.rect.w, exit.rect.h}});
}

PauseSeane::~PauseSeane()
{
}

void PauseSeane::Render()
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

void PauseSeane::Update(float deltaTime)
{
}

void PauseSeane::HandleEvents(SDL_Event *event)
{
    TextElement resume = resumeButtonEntity.get<TextElement>();
    TextElement mainMenu = mainMenuButtonEntity.get<TextElement>();
    TextElement restart = restartButtonEntity.get<TextElement>();
    TextElement exit = exitButtonEntity.get<TextElement>();
    
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT)
    {
        // Only process if this is a new press (state transition from not pressed to pressed)
        if (!prevMouseButtonState)
        {
            prevMouseButtonState = true;
            int mouseX = event->button.x;
            int mouseY = event->button.y;
            
            // Check if the mouse click is within the bounds of the "Resume" button
            if (mouseX >= resume.rect.x && mouseX <= resume.rect.x + resume.rect.w &&
                mouseY >= resume.rect.y && mouseY <= resume.rect.y + resume.rect.h)
            {
                // Resume the game
                if (auto manager = seaneManager.lock())
                {
                    manager->PopSeane(); // Remove the current PauseSeane
                }
            }
            // Check if the mouse click is within the bounds of the "Main Menu" button
            else if (mouseX >= mainMenu.rect.x && mouseX <= mainMenu.rect.x + mainMenu.rect.w &&
                     mouseY >= mainMenu.rect.y && mouseY <= mainMenu.rect.y + mainMenu.rect.h)
            {
                // Go to Main Menu
                if (auto manager = seaneManager.lock())
                {
                    manager->PopSeane();             // Remove the current PauseSeane
                    manager->PushSeane("ManuSeane"); // Push the Main Menu Seane
                }
            }
            else if (mouseX >= restart.rect.x && mouseX <= restart.rect.x + restart.rect.w &&
                     mouseY >= restart.rect.y && mouseY <= restart.rect.y + restart.rect.h)
            {
                // Restart the game
                if (auto manager = seaneManager.lock())
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
                    manager->PopSeane();             // Remove the current PauseSeane
                    manager->PushSeane("PlaySeane"); // Push PlaySeane to restart the game
                }
            }
            else if (mouseX >= exit.rect.x && mouseX <= exit.rect.x + exit.rect.w &&
                     mouseY >= exit.rect.y && mouseY <= exit.rect.y + exit.rect.h)
            {
                SDL_Log("Exit button clicked");
                // Handle exit button click
                if (auto manager = seaneManager.lock())
                {
                    manager->PopSeane(); // Remove the current PauseSeane
                    SDL_Event quitEvent;
                    quitEvent.type = SDL_EVENT_QUIT;
                    SDL_PushEvent(&quitEvent);
                }
            }
        }
    }
    else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT)
    {
        prevMouseButtonState = false;  // Track that button is released
    }
}