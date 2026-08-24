#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include <functional>
#include <vector>
#include <memory>

// Timer component for Flecs
struct TimerComponent
{
    float duration = 0.0f;      // Total duration in seconds
    float elapsed = 0.0f;       // Time elapsed so far
    bool active = true;
    bool loop = false;          // Whether to repeat
    std::function<void()> onComplete = nullptr;
};

// Timer manager to handle all timers in a seane
class TimerManager
{
private:
    struct ScheduledCallback
    {
        std::function<void()> callback;
        float timeRemaining;
        
        ScheduledCallback(std::function<void()> cb, float delay)
            : callback(cb), timeRemaining(delay) {}
    };
    
    std::vector<ScheduledCallback> timerCallbacks;
    
public:
    TimerManager() = default;
    ~TimerManager() = default;
    
    /**
     * Schedule a callback to run after a specified delay
     * @param callback Function to call when timer expires
     * @param delay Time in seconds before callback executes
     */
    void Schedule(std::function<void()> callback, float delay);
    
    /**
     * Update all timers (should be called in Seane::Update)
     * @param deltaTime Time elapsed since last frame in seconds
     */
    void Update(float deltaTime);
    
    /**
     * Clear all scheduled timers
     */
    void Clear();
    
    /**
     * Get number of active timers
     */
    size_t GetActiveTimerCount() const { return timerCallbacks.size(); }
};

#endif // __TIMER_HPP__
