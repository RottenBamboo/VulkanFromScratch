//
// Created by rottenbamboo on 2025/10/13.
//
#include "RBMaterial.h"
#include "RBApplication.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>


namespace RottenBamboo 
{
    RBMaterial::RBMaterial(const std::string& path, RBDevice &device, RBCommandBuffer &commandBuffer)
        : RBResource(path), device(device), commandBuffer(commandBuffer)
    {
        if (!path.empty()) {
            Load(path);
        }
    }

    void RBMaterial::Reset()
    {
        RBLOG_INFO("RBMaterial::Reset()");
        data = RBMaterialData{};
    }

    void RBMaterial::Load(const std::string& loadPath) 
    {
        std::string currentPath;
        if(loadPath.empty())
        {
            RBLOG_INFO("RBMaterial::Load() loadPath is empty. Set to default path : " + path);
            currentPath = path;
        }
        else
        {
            RBLOG_INFO("RBMaterial::Load() loadPath : " + loadPath);
            currentPath = loadPath;
        }

        Reset();

        if (currentPath.empty()) 
        {
            return;
        }

        //load material file
        this->path = currentPath;
        
        std::ifstream materialfile(GET_RESOURCE_ROOT_DIR + this->path);
        if (!materialfile.is_open()) 
        {
            RBLOG_INFO("RBMaterial::Load() could not open: " + this->path);
            std::cout << "RBMaterial::Load() could not open: " << this->path << std::endl;
            return;
        }

        RBMaterialData loaded{};

        try {
            std::stringstream buffer;
            buffer << materialfile.rdbuf();
            const std::string text = buffer.str();
            std::string shaderDefinitionGUID = "";
            FindStringValue(text, "name", loaded.name);
            FindStringValue(text, "shaderDefinationName", shaderDefinitionGUID);
            std::optional<uuids::uuid> id  = uuids::uuid::from_string(shaderDefinitionGUID);
            if(id.has_value())
            {
                std::optional<std::string> shaderDefinitionPath = RBApplication::GetAssetRegistry()->GetPath(id.value());
                if(shaderDefinitionPath.has_value())
                {
                    std::string path = shaderDefinitionPath.value();
                    RemoveExtension(path);
                    loaded.shaderDefinationName = path;
                }
            }
            else
            {
                std::unordered_map<std::string, RBShaderDefinition>* ptr_shaderDefinition = RBApplication::GetShaderDefinition();
                if(ptr_shaderDefinition && !ptr_shaderDefinition->empty())
                {
                    loaded.shaderDefinationName = RBApplication::GetShaderDefinition()->begin()->first;
                }
            }

            RBShaderDefinition* shaderDef = RBApplication::GetShaderDefinition(loaded.shaderDefinationName);
            if(shaderDef == nullptr)
            {
                return;
            }
            loaded.shaderDefinition = *shaderDef;
            
            std::string shaderPathName = RBApplication::GetShaderDefinition(loaded.shaderDefinationName)->GetData().stages[(int)RBShaderStageKind::Fragment].path;
            loaded.shaderReflection = *RBApplication::GetResourceShader()->GetCustomReflection(shaderPathName);
            
            if(loaded.shaderReflection.descriptorSets.size() > 0)
            {
                auto& descriptorSets = loaded.shaderReflection.descriptorSets.at(0);
                for(int i = 0; i < descriptorSets.bindings.size(); i++)
                {
                    if(descriptorSets.bindings.find(i + 1) == descriptorSets.bindings.end())
                    {
                        continue;
                    }

                    RBDescriptorBinding& binding = descriptorSets.bindings.find(i + 1)->second;
                    if(binding.type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
                    {
                        continue;
                    }
                    const std::string& name = binding.name;
                    if(binding.type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER || binding.type == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE)
                    {
                        std::string value;
                        FindStringValue(text, name, value);
                        if(value.empty())
                        {
                            value = defaultWhiteImagePath;
                        }
                        binding.texturePath = value;
                    }
                    else
                    {
                        float value;
                        FindFloatValue(text, name, value);
                        binding.floatValue = value;
                    }
                }
            }

            //data. = std::move(loaded);
            data.name = loaded.name;
            data.shaderDefinationName = loaded.shaderDefinationName;
            data.shaderDefinition = loaded.shaderDefinition;
            data.shaderReflection = loaded.shaderReflection;
            RBLOG_INFO("RBMaterial::Load()");
        }
        catch (const std::exception& e) 
        {
            RBLOG_INFO("RBMaterial::Load() failed for " + this->path + ": " + std::string(e.what()));
            std::cout << "RBMaterial::Load() failed for " << this->path << ": " << e.what() << std::endl;
            Reset();
        }

        //load meta file
        currentPath = GET_RESOURCE_ROOT_DIR + currentPath + META_EXTENSION;
        metaFile.Load(currentPath, data);
    }

    bool RBMaterial::Save(const std::string& savePath, RBMaterialData& materialData)
    {
        std::string currentPath;
        if(savePath.empty())
        {
            RBLOG_INFO("RBMaterial::Save() savePath is empty. Set to default path : " + path);
            currentPath = path;
        }
        else
        {
            RBLOG_INFO("RBMaterial::Save() path : " + path);
            currentPath = savePath;
        }

        bool fileExists;

        //save material file
        std::string matPath = currentPath + MAT_EXTENSION;
        RBLOG_INFO("RBMaterial::Save() targetPath : " + matPath);

        try 
        {
            std::filesystem::path matFilePath(GET_RESOURCE_ROOT_DIR + matPath);
            RBLOG_INFO("RBMaterial::Save() matFilePath : " + matFilePath.string());
            if (matFilePath.has_parent_path()) 
            {
                fileExists = std::filesystem::exists(matFilePath);
                std::filesystem::create_directories(matFilePath.parent_path());
                RBLOG_INFO("RBMaterial::Save() create directories : " + matFilePath.parent_path().string());
            }

            std::ofstream file(matFilePath, std::ios::trunc);
            if (!file.is_open()) {
                RBLOG_INFO("RBMaterial::Save() cannot open file : " + matFilePath.string());
                return false;
            }

            std::string materialFileData = "{\n";
            std::string shaderDefinitionName = "";
            std::optional<uuids::uuid> result = RBApplication::GetAssetRegistry()->GetGuid(GET_RESOURCE_ROOT_DIR + data.shaderDefinationName + META_EXTENSION);
            if(result.has_value())
            {
                shaderDefinitionName = uuids::to_string(result.value());
            }
            materialFileData += "    \"name\": \"" + data.name + "\",\n";
            materialFileData += "    \"shaderDefinationName\": \"" + shaderDefinitionName + "\"";

            if(materialData.shaderReflection.descriptorSets.size() > 0)
            {
                auto& descriptorSets = materialData.shaderReflection.descriptorSets.at(0);
                for(auto itr = descriptorSets.bindings.begin(); itr != descriptorSets.bindings.end(); itr++)
                {
                    if(itr->second.type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
                    {
                        continue;
                    }
                    const std::string &name = itr->second.name;
                    RBLOG_INFO("RBMaterial::Save() name : " + itr->second.name);
                    std::string value = "";
                    if(itr->second.type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER || itr->second.type == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE)
                    {
                        value = itr->second.texturePath;
                        if(value.empty())
                        {
                            value = defaultWhiteImagePath;
                        }
                    
                        RBLOG_INFO("RBMaterial::Save() path : " + value);
                    }
                    else
                    {
                        value = std::to_string(itr->second.floatValue);
                        if(value.empty())
                        {
                            value = "0";
                        }

                        RBLOG_INFO("RBMaterial::Save() float value : " + value);
                    }

                    materialFileData += ",\n    \"" + name + "\": " + "\"" + value + "\"";
                }
            }

            materialFileData += "\n}\n";
            file << materialFileData;
            RBLOG_INFO("RBMaterial::Save() material = " + materialFileData);
        }
        catch (const std::exception& e) 
        {
            std::cout << "RBMaterial::Save() failed for " << matPath << ": " << e.what() << std::endl;
            return false;
        }

        //save material mate file
        std::string matMetaPath = matPath + META_EXTENSION;
        RBLOG_INFO("RBMaterial::Save() targetMetaPath : " + matMetaPath);

        if(!metaFile.Save(matMetaPath, materialData))
        {
            RBLOG_INFO("RBMaterial::Save() failed to save meta file : " + matMetaPath);
            return false;
        }

        data.m_GUID = materialData.m_GUID;

        return true;
    }

    RBMaterialData& RBMaterial::GetData()
    {
        return data;
    }

    const RBMaterialData& RBMaterial::GetData() const
    {
        return data;
    }
}