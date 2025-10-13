//
// Created by rottenbamboo on 2025/9/30.
//

#include "RBResourceManager.h"

namespace RottenBamboo {
    ResourceManager::ResourceManager(RBDevice& device, RBCommandBuffer& commandBuffer)
        : device(device), commandBuffer(commandBuffer) {}

    std::shared_ptr<RBModel> ResourceManager::LoadModels(const std::string& path) {
        std::cout << "ResourceManager::LoadModels() Begin" << std::endl;
        auto it = models.find(path);
        if (it != models.end()) {
            return it->second;
        }

        auto model = std::make_shared<RBModel>(path, device, commandBuffer);
        model->Load(path);
        models[path] = model;
        std::cout << "ResourceManager::LoadModels() End" << std::endl;
        return model;
    }

    std::shared_ptr<RBModel> ResourceManager::GetModel(const std::string& path) {
        std::cout << "ResourceManager::GetModel() Begin" << std::endl;
        auto it = models.find(path);
        if (it != models.end()) {
            return it->second;
        }      
        return nullptr;  
    }

    void ResourceManager::Clear() {
        models.clear();
        resources.clear();
    }
}