//
// Created by rottenbamboo on 2025/9/30.
//

#include "RBResourceManager.h"

namespace RottenBamboo {
    ResourceManager::ResourceManager(RBDevice& device, RBCommandBuffer& commandBuffer)
        : device(device), commandBuffer(commandBuffer) {}

    std::shared_ptr<RBModel> ResourceManager::LoadModel(const std::string& path) {
        auto it = models.find(path);
        if (it != models.end()) {
            return it->second;
        }

        auto model = std::make_shared<RBModel>(device, commandBuffer);
        model->LoadFromFile(path);
        models[path] = model;
        return model;
    }

    void ResourceManager::Clear() {
        models.clear();
    }
}