//
// Created by rottenbamboo on 2026/9/4.
//
#include "RBScene.h"
namespace RottenBamboo
{
    RBScene::RBScene(const std::string &path, const std::string& nodeName, const uuids::uuid& guid) 
     : RBResource(path), m_Root(std::make_unique<RBSceneNode>(nodeName, nullptr, guid)) 
    {

    }

    void RBScene::Load(const std::string& loadPath) 
    {

    }

    bool RBScene::Save(const std::string& savePath, RBSceneData& sceneData)
    {
        std::string currentPath;
        if(savePath.empty())
        {
            RBLOG_INFO("RBScene::Save() savePath is empty. Set to default path : " + path);
            currentPath = path;
        }
        else
        {
            RBLOG_INFO("RBScene::Save() path : " + path);
            currentPath = savePath;
        }
        
        bool fileExists;
        
        //save material file
        std::string scenePath = currentPath + SCENE_EXTENSION;
        RBLOG_INFO("RBScene::Save() targetPath : " + scenePath);
        try 
        {
            std::filesystem::path matFilePath(GET_RESOURCE_ROOT_DIR + scenePath);
            RBLOG_INFO("RBScene::Save() matFilePath : " + matFilePath.string());
            if (matFilePath.has_parent_path()) 
            {
                fileExists = std::filesystem::exists(matFilePath);
                std::filesystem::create_directories(matFilePath.parent_path());
                RBLOG_INFO("RBScene::Save() create directories : " + matFilePath.parent_path().string());
            }

            std::ofstream file(matFilePath, std::ios::trunc);
            if (!file.is_open()) 
            {
                RBLOG_INFO("RBScene::Save() cannot open file : " + matFilePath.string());
                return false;
            }

            std::string sceneFileData = "{\n";
            std::string guidStr = "";
            sceneFileData += "    \"name\": \"" + sceneData.name + "\",\n";
            sceneFileData += "}\n";
            file << sceneFileData;
            RBLOG_INFO("RBScene::Save() scene = " + sceneFileData);
        }
        catch (const std::exception& e)
        {
            std::cout << "RBScene::Save() failed for " << scenePath << ": " << e.what() << std::endl;
            return false;
        }

        //save scene metadata file
        std::string matMetaPath = scenePath + META_EXTENSION;
        RBLOG_INFO("RBScene::Save() targetMetaPath : " + matMetaPath);

        if(!metaFile.Save(matMetaPath, sceneData))
        {
            RBLOG_INFO("RBScene::Save() failed to save meta file : " + matMetaPath);
            return false;
        }

        data.m_GUID = sceneData.m_GUID;
        return true;
    }

    void RBScene::SetRoot(std::unique_ptr<RBSceneNode> root)
    {
        m_Root = std::move(root);
    }

    RBSceneNode* RBScene::GetRoot() const
    {
        return m_Root.get();
    }
}