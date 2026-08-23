#ifndef __GAME_SETUP_HPP__
#define __GAME_SETUP_HPP__

#include <memory>
#include <SDL3/SDL.h>

class SeaneManager;
class AssetsManager;

class GameSetup
{
public:
    static void InitializeSeanes(std::shared_ptr<SeaneManager> seaneManager,
                                 AssetsManager *assetsManager,
                                 SDL_Renderer *renderer);
};

#endif // __GAME_SETUP_HPP__
