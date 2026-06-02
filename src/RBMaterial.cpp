//
// Created by rottenbamboo on 2025/10/13.
//

#include "RBMaterial.h"
#include "RBApplication.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

namespace
{
    bool FindStringValue(const std::string& text, const std::string& key, std::string& out)
    {
        const std::string pattern = "\\\"" + key + "\\\"\\s*:\\s*\\\"([^\\\"]*)\\\"";
        std::regex regexPattern(pattern);
        std::smatch match;
        if (std::regex_search(text, match, regexPattern) && match.size() >= 2)
        {
            out = match[1].str();
            return true;
        }
        return false;
    }

    bool FindFloatValue(const std::string& text, const std::string& key, float& out)
    {
        const std::string pattern = "\\\"" + key + "\\\"\\s*:\\s*([-+]?[0-9]*\\.?[0-9]+(?:[eE][-+]?[0-9]+)?)";
        std::regex regexPattern(pattern);
        std::smatch match;
        if (std::regex_search(text, match, regexPattern) && match.size() >= 2)
        {
            out = std::stof(match[1].str());
            return true;
        }
        return false;
    }

    template <size_t N>
    bool FindArrayValue(const std::string& text, const std::string& key, std::array<float, N>& out)
    {
        const std::string pattern = "\\\"" + key + "\\\"\\s*:\\s*\\[(.*?)\\]";
        std::regex regexPattern(pattern);
        std::smatch match;
        if (!std::regex_search(text, match, regexPattern) || match.size() < 2)
        {
            return false;
        }

        std::stringstream ss(match[1].str());
        std::string token;
        size_t index = 0;
        while (std::getline(ss, token, ',') && index < N)
        {
            out[index++] = std::stof(token);
        }
        return index == N;
    }
}

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
        data = MaterialData{};
    }

    void RBMaterial::Load(const std::string& path) 
    {
        if (!path.empty()) {
            this->path = path;
        }

        Reset();

        if (this->path.empty()) {
            return;
        }

        std::ifstream file(GET_PROJECT_ROOT_DIR + this->path);
        if (!file.is_open()) {
            std::cout << "RBMaterial::Load() could not open: " << this->path << std::endl;
            return;
        }

        try {
            std::stringstream buffer;
            buffer << file.rdbuf();
            const std::string text = buffer.str();

            MaterialData loaded{};
            FindStringValue(text, "name", loaded.name);
            FindStringValue(text, "shaderPathName", loaded.shaderPathName);
            loaded.shaderReflection = RBApplication::GetResourceShader()->GetCustomReflection(loaded.shaderPathName);
            if(loaded.shaderReflection && loaded.shaderReflection->descriptorSets.size() > 0)
            {
                auto& descriptorSets = loaded.shaderReflection->descriptorSets.at(0);
                for(auto itr = descriptorSets.bindings.begin(); itr != descriptorSets.bindings.end(); itr++)
                {
                    if(itr->second.type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
                    {
                        continue;
                    }
                    const std::string& name = itr->second.name;
                    float value;
                    FindFloatValue(text, name, value);
                    itr->second.floatValue = value;
                }
            }

            data = std::move(loaded);
            RBLOG_INFO("RBMaterial::Load()");
        }
        catch (const std::exception& e) {
            std::cout << "RBMaterial::Load() failed for " << this->path << ": " << e.what() << std::endl;
            Reset();
        }
    }

    bool RBMaterial::Save(const std::string& savePath, const MaterialData& materialData) const
    {
        const std::string& targetPath = savePath.empty() ? path : savePath;
        if (targetPath.empty()) {
            return false;
        }

        try {
            std::filesystem::path filePath(GET_PROJECT_ROOT_DIR + targetPath);
            if (filePath.has_parent_path()) {
                std::filesystem::create_directories(filePath.parent_path());
            }

            std::ofstream file(targetPath, std::ios::trunc);
            if (!file.is_open()) {
                return false;
            }

            
            std::string materialFileData = "{\n";
            
            materialFileData += "    \"name\": \"" + data.name + "\",\n";
            materialFileData += "    \"shaderPathName\": \"" + data.shaderPathName + "\",\n";
            if(materialData.shaderReflection && materialData.shaderReflection->descriptorSets.size() > 0)
            {
                auto& descriptorSets = materialData.shaderReflection->descriptorSets.at(0);
                for(auto itr = descriptorSets.bindings.begin(); itr != descriptorSets.bindings.end(); itr++)
                {
                    if(itr->second.type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
                    {
                        continue;
                    }
                    const std::string &name = itr->second.name;
                    std::string value = std::to_string(itr->second.floatValue);
                    if(itr != descriptorSets.bindings.begin())
                    {
                        materialFileData += ",\n";
                    }
                    materialFileData += "    \"" + name + "\": " + value;
                }
            }

            materialFileData += "\n}\n";
            file << materialFileData;
            RBLOG_INFO("RBMaterial::Save()");
            return true;
        }
        catch (const std::exception& e) {
            std::cout << "RBMaterial::Save() failed for " << targetPath << ": " << e.what() << std::endl;
            return false;
        }
    }

    MaterialData& RBMaterial::GetData()
    {
        return data;
    }

    const MaterialData& RBMaterial::GetData() const
    {
        return data;
    }
}