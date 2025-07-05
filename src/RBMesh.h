//
// Created by rottenbamboo on 2023/7/7.
//

#pragma once

#include "RBDevice.h"
#include "RBBuffer.h"

namespace RottenBamboo {

    class RBMesh {

    private:
        RBDevice &device;
        RBCommandBuffer &commandBuffer;

    public:
        RBBuffer<Vertex> vertexBuffer{device, commandBuffer, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT};
        RBBuffer<uint32_t> indexBuffer{device, commandBuffer, VK_BUFFER_USAGE_INDEX_BUFFER_BIT};
        //RBBuffer<UniformBufferShaderVariables> uniformBuffers[MAX_FRAMES_IN_FLIGHT]{{device, commandBuffer, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT}, {device, commandBuffer, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT}};

        RBMesh(RBDevice &device, RBCommandBuffer &commandBuffer);
        ~RBMesh();
        void InitializeMesh();
    };

} // Rottenbamboo

