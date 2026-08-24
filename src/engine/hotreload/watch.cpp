#include <efsw/efsw.hpp>
#include <iostream>
#include <thread>
#include <map>
#include <chrono>

class UpdateListener : public efsw::FileWatchListener
{
private:
    // Track last modification time for each file to debounce events
    std::map<std::string, std::chrono::steady_clock::time_point> lastModifiedTimes;
    const int DEBOUNCE_MS = 100;  // Ignore duplicate events within 100ms

public:
    void handleFileAction(efsw::WatchID watchid, const std::string &dir,
                          const std::string &filename, efsw::Action action,
                          std::string oldFilename) override
    {
        // For Modified events, debounce by checking time since last event
        if (action == efsw::Actions::Modified)
        {
            std::string fullPath = dir + filename;
            auto now = std::chrono::steady_clock::now();
            
            auto it = lastModifiedTimes.find(fullPath);
            if (it != lastModifiedTimes.end())
            {
                auto timeSinceLastEvent = std::chrono::duration_cast<std::chrono::milliseconds>(
                    now - it->second
                ).count();
                
                if (timeSinceLastEvent < DEBOUNCE_MS)
                {
                    // Ignore duplicate event within debounce window
                    return;
                }
            }
            
            lastModifiedTimes[fullPath] = now;
        }

        switch (action)
        {
        case efsw::Actions::Add:
            std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Added"
                      << std::endl;
            break;
        case efsw::Actions::Delete:
            std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Delete"
                      << std::endl;
            break;
        case efsw::Actions::Modified:
            std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Modified"
                      << std::endl;
            break;
        case efsw::Actions::Moved:
            std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Moved from ("
                      << oldFilename << ")" << std::endl;
            break;
        default:
            std::cout << "Should never happen!" << std::endl;
        }
    }
};

// Static pointers to keep the watcher alive
static efsw::FileWatcher *g_fileWatcher = nullptr;
static UpdateListener *g_listener = nullptr;

void intiwatch()
{
    // Create the file system watcher instance
    g_fileWatcher = new efsw::FileWatcher();

    // Create the instance of your efsw::FileWatcherListener implementation
    g_listener = new UpdateListener();

    // Add folders to watch recursively (paths relative to project root, up from build/Debug)
    efsw::WatchID watchID1 = g_fileWatcher->addWatch("../../src/game", g_listener, true);
    efsw::WatchID watchID2 = g_fileWatcher->addWatch("../../include/game", g_listener, true);

    std::cout << "File watcher started for src/game and include/game" << std::endl;

    // Start watching in a separate thread so it doesn't block the main program
    std::thread watchThread([]() {
        std::cout << "Watch thread started" << std::endl;
        g_fileWatcher->watch();
    });
    
    // Detach the thread so it runs independently
    watchThread.detach();
}
