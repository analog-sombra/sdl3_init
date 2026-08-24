#ifndef __HOTRELOAD_WATCH_HPP__
#define __HOTRELOAD_WATCH_HPP__
#include <efsw/efsw.hpp>

class UpdateListener : public efsw::FileWatchListener
{
public:
    void handleFileAction(efsw::WatchID watchid, const std::string &dir,
                          const std::string &filename, efsw::Action action,
                          std::string oldFilename) override;
};
void intiwatch();
#endif // __HOTRELOAD_WATCH_HPP__