//
// Created by rottenbamboo on 2025/10/13.
//

#pragma once

#include "RBDevice.h"
#include "RBBuffer.h"

namespace RottenBamboo {

    class RBShader{
    public:

    private:
        RBDevice &device;
        RBCommandBuffer &commandBuffer;

    public:
        RBShader(RBDevice &device, RBCommandBuffer &commandBuffer);
    };

} // Rottenbamboo

