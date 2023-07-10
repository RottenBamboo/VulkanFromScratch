//
// Created by rottenbamboo on 2023/7/7.
//

#include "RBMesh.h"

namespace RottenBamboo {
    RBMesh::RBMesh(RBDevice &device, RBCommandBuffer &commandBuffer) : device(device), commandBuffer(commandBuffer)
    {
    }
    RBMesh::~RBMesh()
    {
        vertexBuffer.ReleaseBuffer();
        indexBuffer.ReleaseBuffer();
    }

    void RBMesh::InitializeMesh()
    {
        vertexBuffer.CreateBuffer();
        indexBuffer.CreateBuffer();
    }
}