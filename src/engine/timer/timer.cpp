#include "engine/timer/timer.hpp"
#include <algorithm>

void TimerManager::Schedule(std::function<void()> callback, float delay)
{
    if (delay < 0.0f)
        delay = 0.0f;
    
    timerCallbacks.emplace_back(callback, delay);
}

void TimerManager::Update(float deltaTime)
{
    // Update all timers
    for (auto& timer : timerCallbacks)
    {
        timer.timeRemaining -= deltaTime;
        
        // Timer expired, call callback
        if (timer.timeRemaining <= 0.0f && timer.callback)
        {
            timer.callback();
        }
    }
    
    // Remove expired timers
    timerCallbacks.erase(
        std::remove_if(timerCallbacks.begin(), timerCallbacks.end(),
            [](const ScheduledCallback& timer) { return timer.timeRemaining <= 0.0f; }),
        timerCallbacks.end()
    );
}

void TimerManager::Clear()
{
    timerCallbacks.clear();
}
