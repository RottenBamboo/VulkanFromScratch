#pragma once
#include <efsw/efsw.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include "RBAssetsRegistry.h"
#include "RBLogger.h"
namespace RottenBamboo 
{
    class RBAssetsRegistryListener : public efsw::FileWatchListener 
    {
      public:
        void handleFileAction( efsw::WatchID watchid, const std::string& dir,
                               const std::string& filename, efsw::Action action,
                               const std::string& oldFilename ) override 
        {
            std::string fullPath = dir + filename;

            switch (action) 
            {
                case efsw::Actions::Add:
                    std::cout << "[Add] " << fullPath << std::endl;
                    RBLOG_INFO( "[Add] " + fullPath);
                    break;
                case efsw::Actions::Delete:
                    std::cout << "[Delete] " << fullPath << std::endl;
                    RBLOG_INFO( "[Delete] " + fullPath);
                    break;
                case efsw::Actions::Modified:
                    std::cout << "[Modified] " << fullPath << std::endl;
                    RBLOG_INFO( "[Modified] " + fullPath);
                    break;
                case efsw::Actions::Moved:
                    std::cout << "[Moved] " << oldFilename << " -> " << fullPath << std::endl;
                    RBLOG_INFO( "[Moved] " + oldFilename + " -> " + fullPath);
                    break;
                default:
                    break;
            }
        }
        
    private:
        RBAssetsRegistry* m_Registry;
    };
}