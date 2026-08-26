#pragma once
#include <efsw/efsw.hpp>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include "RBAssetsRegistry.h"
#include "RBMetaFile.h"
namespace RottenBamboo 
{
    class RBAssetsRegistryListener : public efsw::FileWatchListener 
    {
      public:
        RBAssetsRegistryListener(std::queue<std::string>& queue, std::mutex& mtx);
        void handleFileAction( efsw::WatchID watchid, const std::string& dir,
                               const std::string& filename, efsw::Action action,
                               const std::string& oldFilename ) override;
        
    private:
        std::queue<std::string>& m_EventQueue;
        std::mutex& m_QueueMutex;
        RBMetaFile m_metaFile;
        RBAssetsRegistry* m_Registry;
    };
}