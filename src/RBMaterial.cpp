//
// Created by rottenbamboo on 2025/10/13.
//

#include "RBMaterial.h"

namespace RottenBamboo 
{
    RBMaterial::RBMaterial(const std::string& path, RBDevice &device, RBCommandBuffer &commandBuffer) 
    : RBResource(path), device(device), commandBuffer(commandBuffer)
    {

    }
    void RBMaterial::Load(const std::string& path) 
    {
    }
}