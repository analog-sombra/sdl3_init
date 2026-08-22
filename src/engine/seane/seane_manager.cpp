#include "engine/seane/seane_manager.hpp"
#include <algorithm>

SeaneManager::SeaneManager(AssetsManager *assetsManager, SDL_Renderer *renderer)
    : assetsManager(assetsManager), renderer(renderer)
{
    SDL_Log("SeaneManager initialized");
}

SeaneManager::~SeaneManager()
{
    ClearAllSeanes();
    SDL_Log("SeaneManager destroyed");
}

void SeaneManager::AddSeane(const std::string &name, std::unique_ptr<Seane> seane)
{
    if (seanes.find(name) != seanes.end())
    {
        SDL_Log("Seane '%s' already exists", name.c_str());
        return;
    }
    
    seanes[name] = std::move(seane);
    SDL_Log("Seane '%s' added to manager", name.c_str());
}

void SeaneManager::DeleteSeane(const std::string &name)
{
    auto it = seanes.find(name);
    if (it == seanes.end())
    {
        SDL_Log("Seane '%s' not found", name.c_str());
        return;
    }
    
    seanes.erase(it);
    SDL_Log("Seane '%s' deleted", name.c_str());
}

void SeaneManager::ClearAllSeanes()
{
    seanes.clear();
    
    // Clear the stack
    while (!seaneStack.empty())
    {
        seaneStack.pop();
    }
    
    SDL_Log("All seanes cleared");
}



void SeaneManager::PushSeane(const std::string &name)
{
    auto it = seanes.find(name);
    if (it == seanes.end())
    {
        SDL_Log("Seane '%s' not found", name.c_str());
        return;
    }
    
    seaneStack.push(name);
    SDL_Log("Seane '%s' pushed to stack (stack size: %d)", name.c_str(), (int)seaneStack.size());
}

void SeaneManager::PopSeane()
{
    if (seaneStack.empty())
    {
        SDL_Log("Seane stack is empty, cannot pop");
        return;
    }
    
    std::string poppedSeane = seaneStack.top();
    seaneStack.pop();
    
    if (seaneStack.empty())
    {
        SDL_Log("Seane '%s' popped. Stack is now empty", poppedSeane.c_str());
    }
    else
    {
        std::string topSeane = seaneStack.top();
        SDL_Log("Seane '%s' popped. Returned to seane '%s'", poppedSeane.c_str(), topSeane.c_str());
    }
}

Seane *SeaneManager::PeekSeane() const
{
    if (seaneStack.empty())
    {
        return nullptr;
    }
    
    std::string topSeane = seaneStack.top();
    auto it = seanes.find(topSeane);
    if (it != seanes.end())
    {
        return it->second.get();
    }
    
    return nullptr;
}

int SeaneManager::GetStackSize() const
{
    return seaneStack.size();
}



std::vector<std::string> SeaneManager::GetAllSeanes() const
{
    std::vector<std::string> seaneNames;
    for (const auto &pair : seanes)
    {
        seaneNames.push_back(pair.first);
    }
    return seaneNames;
}

std::vector<std::string> SeaneManager::GetAllStackSeanes() const
{
    std::vector<std::string> stackSeanes;
    
    // Convert stack to vector (bottom to top)
    std::stack<std::string> tempStack = seaneStack;
    while (!tempStack.empty())
    {
        stackSeanes.insert(stackSeanes.begin(), tempStack.top());
        tempStack.pop();
    }
    
    return stackSeanes;
}

bool SeaneManager::HasSeane(const std::string &name) const
{
    return seanes.find(name) != seanes.end();
}

bool SeaneManager::IsSeaneInStack(const std::string &name) const
{
    std::stack<std::string> tempStack = seaneStack;
    while (!tempStack.empty())
    {
        if (tempStack.top() == name)
        {
            return true;
        }
        tempStack.pop();
    }
    return false;
}

int SeaneManager::GetSeaneCount() const
{
    return seanes.size();
}

void SeaneManager::Render()
{
    // Render all seanes in the stack (bottom to top) for layering effect
    // This allows pause menu to render on top of gameplay
    std::vector<std::string> stackSeanes = GetAllStackSeanes();
    for (const auto &seaneName : stackSeanes)
    {
        auto it = seanes.find(seaneName);
        if (it != seanes.end())
        {
            it->second->Render();
        }
    }
}

void SeaneManager::Update()
{
    // Only update the top seane in the stack
    Seane *topSeane = PeekSeane();
    if (topSeane != nullptr)
    {
        topSeane->Update();
    }
}

void SeaneManager::HandleEvents(SDL_Event *event)
{
    // Only top seane handles events (so pause menu captures input)
    Seane *topSeane = PeekSeane();
    if (topSeane != nullptr)
    {
        topSeane->HandleEvents(event);
    }
}

