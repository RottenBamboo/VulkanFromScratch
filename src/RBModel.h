//
// Created by rottenbamboo on 2025/9/30.
//

#pragma once

#include "RBMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace RottenBamboo {
    
    class RBModel {
    public:
        void loadModelFromFile(const std::string& path);

        void transformModelVertex(aiMesh* meshPtr, 
                                          std::vector<Vertex>& vertexBuffer, 
                                          int& vertexWriteIndex, 
                                          const aiMatrix4x4& transform);

        void processModelNode(const aiNode* node,
                         const aiScene* scene,
                         std::vector<Vertex>& vertexBuffer,
                         std::vector<uint32_t>& indexBuffer,
                         int& vertexWriteIndex,
                         int& vertexStartOffset,
                         const aiMatrix4x4& parentTransform);

    private:
        RBDevice &device;
        RBCommandBuffer &commandBuffer;
        std::vector<std::unique_ptr<RBMesh>> meshes;

    public:
        

        RBModel(RBDevice &device, RBCommandBuffer &commandBuffer);
        ~RBModel();
    };
} // Rottenbamboo
