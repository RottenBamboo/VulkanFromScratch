//
// Created by rottenbamboo on 2025/10/13.
//

#pragma once

#include "RBResource.h"
#include "RBDevice.h"
#include "RBBuffer.h"

namespace RottenBamboo {

    class RBMaterial : public RBResource {
    public:
        void Load(const std::string& path) override;

    private:
    private:
        RBDevice &device;
        RBCommandBuffer &commandBuffer;

    public:
        RBMaterial(const std::string& path, RBDevice &device, RBCommandBuffer &commandBuffer);
    };

} // Rottenbamboo

