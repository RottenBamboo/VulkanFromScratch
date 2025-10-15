//
// Created by rottenbamboo on 2023/7/7.
//

#pragma once

#include "RBResource.h"
#include "RBDevice.h"
#include "RBBuffer.h"

namespace RottenBamboo {

    class RBMesh : public RBResource{
    public:
        void Load(const std::string& path) override;
        
    private:
        RBDevice &device;
        RBCommandBuffer &commandBuffer;

    public:
        RBBuffer<Vertex> vertexBuffer{device, commandBuffer, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT};
        RBBuffer<uint32_t> indexBuffer{device, commandBuffer, VK_BUFFER_USAGE_INDEX_BUFFER_BIT};
        uint32_t indexCount{0};

        RBMesh(const std::string& path, RBDevice &device, RBCommandBuffer &commandBuffer);
        //~RBMesh() = default;
        void InitializeMesh();
        void InitializeMeshBuffer();
        void AddMeshData(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    };

} // Rottenbamboo

