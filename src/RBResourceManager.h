#pragma once
#include "RBModel.h"
#include <unordered_map>

namespace RottenBamboo {

    class ResourceManager {
    public:
        ResourceManager(RBDevice& device, RBCommandBuffer& commandBuffer);

        std::shared_ptr<RBModel> LoadModel(const std::string& path);

        void Clear();

    private:
        RBDevice& device;
        RBCommandBuffer& commandBuffer;
        std::unordered_map<std::string, std::shared_ptr<RBModel>> models;
    };
}