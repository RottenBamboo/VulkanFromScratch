//
// Created by rottenbamboo on 2026/8/27.
//
#include <iostream>
#include "RBAssetsRegistryListener.h"
#include "RBApplication.h"
#include "RBLogger.h"
namespace RottenBamboo 
{
    RBAssetsRegistryListener::RBAssetsRegistryListener(std::queue<std::string>& queue, std::mutex& mtx)
            : m_EventQueue(queue), m_QueueMutex(mtx) {}

    void RBAssetsRegistryListener::handleFileAction( efsw::WatchID watchid, const std::string& dir,
                               const std::string& filename, efsw::Action action,
                               const std::string& oldFilename ) 
    {
        std::string fullPath = dir + filename;
        std::filesystem::path metaPath = fullPath;
        RBData data{};
        bool isMetaFile = false;
        if(metaPath.has_extension() && metaPath.extension() == ".meta")
        {
            m_metaFile.Load(fullPath, data);
            isMetaFile = true;
        }
        switch (action) 
        {
            case efsw::Actions::Add:
                if(isMetaFile)
                {
                    RBApplication::GetAssetRegistry()->Register(data.m_GUID, fullPath);
                }
                std::cout << "[Add] " << fullPath << std::endl;
                RBLOG_INFO( "[Add] " + fullPath);
                break;
            case efsw::Actions::Delete:
                if(isMetaFile)
                {
                    RBApplication::GetAssetRegistry()->Unregister(data.m_GUID);
                }
                std::cout << "[Delete] " << fullPath << std::endl;
                RBLOG_INFO( "[Delete] " + fullPath);
                break;
            case efsw::Actions::Modified:
                if(isMetaFile)
                {
                    RBApplication::GetAssetRegistry()->Register(data.m_GUID, fullPath);
                }
                std::cout << "[Modified] " << fullPath << std::endl;
                RBLOG_INFO( "[Modified] " + fullPath);
                break;
            case efsw::Actions::Moved:
                if(isMetaFile)
                {
                    RBApplication::GetAssetRegistry()->Register(data.m_GUID, fullPath);
                }
                std::cout << "[Moved] " << oldFilename << " -> " << fullPath << std::endl;
                RBLOG_INFO( "[Moved] " + oldFilename + " -> " + fullPath);
                break;
            default:
                break;
        }
    }
}