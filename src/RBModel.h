//
// Created by rottenbamboo on 2025/9/30.
//

#pragma once

#include "RBMesh.h"

namespace RottenBamboo {
    
    class RBModel {
    public:
        void LoadFromFile(const std::string& path);

    private:
        RBDevice &device;
        RBCommandBuffer &commandBuffer;
        std::vector<std::unique_ptr<RBMesh>> meshes;

    public:
        

        RBModel(RBDevice &device, RBCommandBuffer &commandBuffer);
        ~RBModel();
    };
}
