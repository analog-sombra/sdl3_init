#ifndef __SEANE_FACTORY_HPP__
#define __SEANE_FACTORY_HPP__

#include <memory>
#include <SDL3/SDL.h>
#include "engine/seane/seane.hpp"

class AssetsManager;

/**
 * SeaneFactory
 * 
 * Responsible for creating all seane instances.
 * This keeps seane instantiation decoupled from the engine.
 * Add new seanes here when you create them.
 */
class SeaneFactory
{
public:
    /**
     * Creates a TestSeane instance
     * @param assetsManager Pointer to the assets manager
     * @param renderer Pointer to the SDL renderer
     * @return std::unique_ptr to the newly created TestSeane
     */
    static std::unique_ptr<Seane> CreateTestSeane(AssetsManager* assetsManager, SDL_Renderer* renderer);

    // Add more seane creation methods here as you create new seanes
    // static std::unique_ptr<Seane> CreatePauseSeane(AssetsManager* assetsManager, SDL_Renderer* renderer);
    // static std::unique_ptr<Seane> CreateMenuSeane(AssetsManager* assetsManager, SDL_Renderer* renderer);
    // static std::unique_ptr<Seane> CreateSettingsSeane(AssetsManager* assetsManager, SDL_Renderer* renderer);
};

#endif // __SEANE_FACTORY_HPP__
