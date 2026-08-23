#ifndef __SEANE_MANAGER_HPP__
#define __SEANE_MANAGER_HPP__

#include "engine/assets_manager.hpp"
#include "seane.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <map>
#include <vector>
#include <stack>

class SeaneManager; // Forward declaration for shared_ptr in Seane

class SeaneManager
{
public:
    SeaneManager(AssetsManager *assetsManager, SDL_Renderer *renderer);
    ~SeaneManager();
    
    // Add and manage seanes
    void AddSeane(const std::string &name, std::unique_ptr<Seane> seane, std::shared_ptr<SeaneManager> manager = nullptr);
    void DeleteSeane(const std::string &name);
    void ClearAllSeanes();
    
    // Stack-based seane management (for layering like pause menu)
    void PushSeane(const std::string &name);
    void PopSeane();
    Seane *PeekSeane() const;  // Get top seane without popping
    int GetStackSize() const;
    

    
    // Utility methods
    std::vector<std::string> GetAllSeanes() const;  // Return all registered seanes
    std::vector<std::string> GetAllStackSeanes() const;  // Return all seanes in stack (bottom to top)
    bool HasSeane(const std::string &name) const;
    int GetSeaneCount() const;
    bool IsSeaneInStack(const std::string &name) const;
    
    // Main update/render/handle events
    // Renders ALL seanes in stack (bottom to top)
    void Render();
    // Only updates top seane in stack
    void Update();
    // Only top seane handles events
    void HandleEvents(SDL_Event *event);

private:
    AssetsManager *assetsManager;
    SDL_Renderer *renderer;
    std::map<std::string, std::unique_ptr<Seane>> seanes;
    std::stack<std::string> seaneStack;
};

#endif // __SEANE_MANAGER_HPP__