#pragma once

#include "RBMetaFile.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

namespace RottenBamboo
{
    bool RBMetaFile::Load(const std::string& loadPath, RBData& data)
    { //load meta file
        const std::string currentPath = loadPath;
        std::ifstream metafile(currentPath);

        if (!metafile.is_open()) 
        {
            RBLOG_INFO("RBMaterial::Load() could not open meta file: " + currentPath);
            std::cout << "RBMaterial::Load() could not open meta file: " << currentPath << std::endl;
            
            RBLOG_INFO("RBMaterial::Load() Regenerate meta file " + currentPath);
            std::cout << "RBMaterial::Load() Regenerate meta file  " << currentPath << std::endl;
            
            //regenerate meta file
            std::filesystem::path matMetaFilePath(currentPath);
            std::filesystem::create_directories(matMetaFilePath.parent_path());

            std::ofstream outFile(matMetaFilePath, std::ios::trunc);
            std::string materialMetaFileData = "{\n";

            std::string uuidStrting;
            if(data.IsGUIDNil())
            {
                data.GenerateGUID();
            }
            data.GUIDToString(uuidStrting);
            materialMetaFileData += "    \"uuid\": \"" + uuidStrting + "\"";

            materialMetaFileData += "\n}\n";
            outFile << materialMetaFileData;
            outFile.close();
            RBLOG_INFO("RBMaterial::Load() materialMeta = " + materialMetaFileData);
            return true;
        }

        try {
            std::stringstream buffer;
            buffer << metafile.rdbuf();
            const std::string text = buffer.str();
            std::string uuidString;
            FindStringValue(text, "uuid", uuidString);
            if(!data.GenerateGUIDFromString(uuidString))
            {
                data.GenerateGUID();
                std::string logGUID;
                data.GUIDToString(logGUID);
                RBLOG_INFO("There is no valid UUID in :" + currentPath + ". Regenerate UUID : " + logGUID);
                std::cout << "There is no valid UUID in :" << currentPath << ". Regenerate UUID : " << logGUID << std::endl;
            }
            //data.m_GUID = loaded.m_GUID;
            RBLOG_INFO("RBMaterial::Load()");
            return true;
        }
        catch (const std::exception& e) 
        {
            RBLOG_INFO("RBMaterial::Load() failed for " + currentPath + ": " + std::string(e.what()));
            std::cout << "RBMaterial::Load() failed for " << currentPath << ": " << e.what() << std::endl;
        }
        
        return false;
    }
    
    bool RBMetaFile::Save(const std::string& savePath, RBData& data)
    {
        bool fileExists;
        try
        {
            std::filesystem::path matMetaFilePath(GET_RESOURCE_ROOT_DIR + savePath);
            RBLOG_INFO("RBMaterial::Save() matMetaFilePath : " + matMetaFilePath.string());
            if (matMetaFilePath.has_parent_path()) 
            {
                fileExists = std::filesystem::exists(matMetaFilePath);
                std::filesystem::create_directories(matMetaFilePath.parent_path());
                RBLOG_INFO("RBMaterial::Save() create directories : " + matMetaFilePath.parent_path().string());
            }

            std::ofstream file(matMetaFilePath, std::ios::trunc);
            if (!file.is_open()) {
                RBLOG_INFO("RBMaterial::Save() cannot open file : " + matMetaFilePath.string());
                return false;
            }
            
            std::string materialMetaFileData = "{\n";

            std::string uuidStrting;
            if(data.IsGUIDNil())
            {
                data.GenerateGUID();
            }
            data.GUIDToString(uuidStrting);
            materialMetaFileData += "    \"uuid\": \"" + uuidStrting + "\"";

            materialMetaFileData += "\n}\n";
            file << materialMetaFileData;
            RBLOG_INFO("RBMaterial::Save() material = " + materialMetaFileData);
            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
        return false;
    }
    RBMetaFile::~RBMetaFile()
    {
    }
    RBMetaFile::RBMetaFile()
    {
    }
}