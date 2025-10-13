//
// Created by rottenbamboo on 2023/7/7.
//

#include "RBMesh.h"

namespace RottenBamboo {
    RBMesh::RBMesh(RBDevice &device, RBCommandBuffer &commandBuffer) : device(device), commandBuffer(commandBuffer)
    {
    }
    // RBMesh::~RBMesh()
    // {
    //     vertexBuffer.ReleaseBuffer();
    //     indexBuffer.ReleaseBuffer();
    //     indexCount = 0;
    // }

    void RBMesh::InitializeMesh()
    {
        vertexBuffer.CreateBuffer();
        indexBuffer.CreateBuffer();
        indexCount = static_cast<uint32_t>(indexBuffer.data.size());
    }

    void RBMesh::InitializeMeshBuffer()
    {
        vertexBuffer.CreateBuffer();
        indexBuffer.CreateBuffer();
    }

    void RBMesh::AddMeshData(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    {
        vertexBuffer.data = vertices;
        indexBuffer.data = indices;
        indexCount = static_cast<uint32_t>(indices.size());
    }

    void RBMesh::Load(const std::string& path) 
    {
    }
} // Rottenbamboo