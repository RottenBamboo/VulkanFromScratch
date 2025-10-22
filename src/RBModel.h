//
// Created by rottenbamboo on 2025/9/30.
//

#pragma once

#include "RBMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifdef __ANDROID__
#include <SDL3/SDL_iostream.h>
#endif

namespace RottenBamboo {
    
    class RBModel : public RBResource {
    public:
        void Load(const std::string& path) override;

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

        std::unique_ptr<RBMesh>& getMeshes(int index);

    private:
        RBDevice &device;
        RBCommandBuffer &commandBuffer;
        std::vector<std::unique_ptr<RBMesh>> meshes;


    public:
    
        RBModel(const std::string &path, RBDevice &device, RBCommandBuffer &commandBuffer);
        ~RBModel() = default;

    private:
    // file extension check
    std::string GetFileExtension(const std::string& filename) {
        size_t dotPos = filename.find_last_of(".");
        if (dotPos != std::string::npos && dotPos < filename.length() - 1) {
            return filename.substr(dotPos + 1);
        }
        return "";
    }

#ifdef __ANDROID__
    std::string Base64Encode(const std::vector<uint8_t>& data);
    void ProcessScene(const aiScene* scene, const std::string& filepath);   
    bool TryLoadGLTFWithManualBin(const std::string& gltfPath);
#endif

    };
} // Rottenbamboo
