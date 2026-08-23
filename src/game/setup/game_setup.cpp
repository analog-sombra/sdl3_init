#include "game/setup/game_setup.hpp"
#include "game/seanes/seane_factory.hpp"
#include "engine/seane/seane_manager.hpp"
#include "game/seanes/testseane.hpp"

void GameSetup::InitializeSeanes(std::shared_ptr<SeaneManager> seaneManager,
                                 AssetsManager *assetsManager,
                                 SDL_Renderer *renderer)
{
    // Create and add all seanes here
    // Add TestSeane
    seaneManager->AddSeane(
        "TestSeane",
        std::make_unique<TestSeane>(assetsManager, renderer),
        // SeaneFactory::CreateTestSeane(assetsManager, renderer),
        seaneManager);

    // Add more seanes here as you create them
    // seaneManager->AddSeane(
    //     "PauseSeane",
    //     SeaneFactory::CreatePauseSeane(assetsManager, renderer),
    //     seaneManager
    // );
    // seaneManager->AddSeane(
    //     "MenuSeane",
    //     SeaneFactory::CreateMenuSeane(assetsManager, renderer),
    //     seaneManager
    // );

    // Push the initial seane to start the game
    seaneManager->PushSeane("TestSeane");

    SDL_Log("Game seanes initialized successfully");
}
