//
// Created by rottenbamboo on 2026/9/4.
//
#include "RBPrefab.h"
#include "uuid.h"
#include "RBCommon.h"
namespace RottenBamboo
{
    RBPrefab::RBPrefab(const std::string& path, const RBObjectData& data)
        : RBResource(path), prefabData(data)
    {
    }

    RBPrefab::~RBPrefab()
    {

    }
     void RBPrefab::Load(const std::string& path)
     {

     }

    bool RBPrefab::Save(const std::string& savePath, RBObjectData& data)
    {
        std::string currentPath;
        if(savePath.empty())
        {
            RBLOG_INFO("RBPrefab::Save() savePath is empty. Set to default path : " + path);
            currentPath = path;
        }
        else
        {
            RBLOG_INFO("RBPrefab::Save() path : " + path);
            currentPath = savePath;
        }
        
        bool fileExists;

        //save prefab file
        std::string prefabPath = currentPath + PREFAB_EXTENSION;
        RBLOG_INFO("RBPrefab::Save() targetPath : " + prefabPath);

        try 
        {
            std::filesystem::path prefabFilePath(GET_RESOURCE_ROOT_DIR + prefabPath);
            RBLOG_INFO("RBPrefab::Save() prefabFilePath : " + prefabFilePath.string());
            if (prefabFilePath.has_parent_path()) 
            {
                fileExists = std::filesystem::exists(prefabFilePath);
                std::filesystem::create_directories(prefabFilePath.parent_path());
                RBLOG_INFO("RBPrefab::Save() create directories : " + prefabFilePath.parent_path().string());
            }

            std::ofstream file(prefabFilePath, std::ios::trunc);
            if (!file.is_open()) {
                RBLOG_INFO("RBPrefab::Save() cannot open file : " + prefabFilePath.string());
                return false;
            }

            std::string prefabFileData = "{\n";
            prefabFileData += "    \"name\": \"" + data.name + "\",\n";

            prefabFileData += "\n}\n";
            file << prefabFileData;
            RBLOG_INFO("RBPrefab::Save() prefab = " + prefabFileData);
        }
        catch (const std::exception& e) 
        {
            std::cout << "RBPrefab::Save() failed for " << prefabPath << ": " << e.what() << std::endl;
            return false;
        }

        //save prefab meta file
        std::string prefabMetaPath = prefabPath + META_EXTENSION;
        RBLOG_INFO("RBPrefab::Save() targetMetaPath : " + prefabMetaPath);

        if(!metaFile.Save(prefabMetaPath, prefabData))
        {
            RBLOG_INFO("RBPrefab::Save() failed to save meta file : " + prefabMetaPath);
            return false;
        }

        prefabData.m_GUID = data.m_GUID;

        return true;
    }
}