//
// Created by rottenbamboo on 2025/10/13.
//

#include "RBMaterial.h"

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

        std::ifstream file(this->path);
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
            FindArrayValue(text, "baseColor", loaded.baseColor);
            FindArrayValue(text, "emissiveColor", loaded.emissiveColor);
            FindFloatValue(text, "metallic", loaded.metallic);
            FindFloatValue(text, "roughness", loaded.roughness);
            FindFloatValue(text, "emissiveStrength", loaded.emissiveStrength);
            FindFloatValue(text, "alphaCutoff", loaded.alphaCutoff);
            FindStringValue(text, "baseColorTexture", loaded.baseColorTexture);
            FindStringValue(text, "normalTexture", loaded.normalTexture);
            FindStringValue(text, "metallicRoughnessTexture", loaded.metallicRoughnessTexture);
            FindStringValue(text, "emissiveTexture", loaded.emissiveTexture);

            data = std::move(loaded);
        }
        catch (const std::exception& e) {
            std::cout << "RBMaterial::Load() failed for " << this->path << ": " << e.what() << std::endl;
            Reset();
        }
    }

    bool RBMaterial::Save(const std::string& savePath) const
    {
        const std::string& targetPath = savePath.empty() ? path : savePath;
        if (targetPath.empty()) {
            return false;
        }

        try {
            std::filesystem::path filePath(targetPath);
            if (filePath.has_parent_path()) {
                std::filesystem::create_directories(filePath.parent_path());
            }

            std::ofstream file(targetPath, std::ios::trunc);
            if (!file.is_open()) {
                return false;
            }

              file << "{\n"
                  << "    \"name\": \"" << data.name << "\",\n"
                  << "    \"baseColor\": [" << data.baseColor[0] << ", " << data.baseColor[1] << ", " << data.baseColor[2] << ", " << data.baseColor[3] << "],\n"
                  << "    \"emissiveColor\": [" << data.emissiveColor[0] << ", " << data.emissiveColor[1] << ", " << data.emissiveColor[2] << "],\n"
                  << "    \"metallic\": " << data.metallic << ",\n"
                  << "    \"roughness\": " << data.roughness << ",\n"
                  << "    \"emissiveStrength\": " << data.emissiveStrength << ",\n"
                  << "    \"alphaCutoff\": " << data.alphaCutoff << ",\n"
                  << "    \"baseColorTexture\": \"" << data.baseColorTexture << "\",\n"
                  << "    \"normalTexture\": \"" << data.normalTexture << "\",\n"
                  << "    \"metallicRoughnessTexture\": \"" << data.metallicRoughnessTexture << "\",\n"
                  << "    \"emissiveTexture\": \"" << data.emissiveTexture << "\"\n"
                  << "}\n";
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