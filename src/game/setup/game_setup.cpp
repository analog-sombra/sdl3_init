#include "game/setup/game_setup.hpp"
#include "engine/seane/seane_manager.hpp"
#include "game/seanes/testseane.hpp"
#include "game/seanes/splashseane.hpp"
#include "game/seanes/manuseane.hpp"
#include "game/seanes/pauseseane.hpp"
#include "game/seanes/playseane.hpp"
#include "game/seanes/gameoverseane.hpp"

void GameSetup::InitializeSeanes(std::shared_ptr<SeaneManager> seaneManager,
                                 AssetsManager *assetsManager,
                                 SDL_Renderer *renderer)
{
    // Create and add all seanes here
    // Add TestSeane
    // seaneManager->AddSeane(
    //     "TestSeane",
    //     std::make_unique<TestSeane>(assetsManager, renderer),
    //     seaneManager);

    // Add SplashSeane
    seaneManager->AddSeane(
        "SplashSeane",
        std::make_unique<SplashSeane>(assetsManager, renderer),
        seaneManager);
    
    // Add ManuSeane
    seaneManager->AddSeane(
        "ManuSeane",
        std::make_unique<ManuSeane>(assetsManager, renderer),
        seaneManager);
    
    // Add PlaySeane
    seaneManager->AddSeane(
        "PlaySeane",
        std::make_unique<PlaySeane>(assetsManager, renderer),
        seaneManager);
    
    // Add PauseSeane
    seaneManager->AddSeane(
        "PauseSeane",
        std::make_unique<PauseSeane>(assetsManager, renderer),
        seaneManager);
    
    // Add GameOverSeane with score 0 initially (will be created with actual score when needed)
    seaneManager->AddSeane(
        "GameOverSeane",
        std::make_unique<GameOverSeane>(assetsManager, renderer, 0),
        seaneManager);

    // Push the initial seane to start the game
    seaneManager->PushSeane("ManuSeane");
}
