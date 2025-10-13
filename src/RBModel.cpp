//
// Created by rottenbamboo on 2025/9/30.
//

#include "RBModel.h"

namespace RottenBamboo {

    RBModel::RBModel(const std::string &path, RBDevice &device, RBCommandBuffer &commandBuffer) 
    : RBResource(path), device(device), commandBuffer(commandBuffer)
    {
    }
    
    void RBModel::transformModelVertex(
    aiMesh* meshPtr, 
    std::vector<Vertex>& vertexBuffer, 
    int& vertexWriteIndex, 
    const aiMatrix4x4& transform)
    {
        aiMatrix3x3 normalMatrix = aiMatrix3x3(transform);
        normalMatrix.Inverse().Transpose();

        for (unsigned int v = 0; v < meshPtr->mNumVertices; ++v) 
        {
            aiVector3D vertex = meshPtr->mVertices[v];
            vertex *= transform;  //transform the vertex position

            vertexBuffer[vertexWriteIndex].pos = { vertex.x, vertex.y, vertex.z };
            vertexBuffer[vertexWriteIndex].color = {1.0f, 1.0f, 1.0f};

            if (meshPtr->HasTangentsAndBitangents()) 
            {
                aiVector3D tangent = meshPtr->mTangents[v];
                tangent = normalMatrix * tangent;
                vertexBuffer[vertexWriteIndex].tangent = glm::vec3(tangent.x, tangent.y, tangent.z);
            }
            else 
            {
                vertexBuffer[vertexWriteIndex].tangent = {0.0f, 0.0f, 0.0f};
            }

            if (meshPtr->HasNormals()) 
            {
                aiVector3D normal = meshPtr->mNormals[v];
                normal = normalMatrix * normal;
                vertexBuffer[vertexWriteIndex].normal = glm::vec3(normal.x, normal.y, normal.z);
            }
            else 
            {
                vertexBuffer[vertexWriteIndex].normal = {0.0f, 0.0f, 0.0f};
            }

            if (meshPtr->HasTextureCoords(0)) 
            {
                vertexBuffer[vertexWriteIndex].texCoord = 
                {
                    meshPtr->mTextureCoords[0][v].x,
                    meshPtr->mTextureCoords[0][v].y
                };
            } 
            else 
            {
                vertexBuffer[vertexWriteIndex].texCoord = {0.0f, 0.0f};
            }

            vertexWriteIndex++;
        }
    }

    void RBModel::processModelNode(
    const aiNode* node,
    const aiScene* scene,
    std::vector<Vertex>& vertexBuffer,
    std::vector<uint32_t>& indexBuffer,
    int& vertexWriteIndex,
    int& vertexStartOffset,
    const aiMatrix4x4& parentTransform)
    {
        aiMatrix4x4 currentTransform = parentTransform * node->mTransformation;

        for (unsigned int i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh* meshPtr = scene->mMeshes[node->mMeshes[i]];

            // vertex push back
            transformModelVertex(meshPtr, vertexBuffer, vertexWriteIndex, currentTransform);

            // index push back
            for (unsigned int f = 0; f < meshPtr->mNumFaces; ++f) 
            {
                aiFace& face = meshPtr->mFaces[f];
                if (face.mNumIndices != 3) continue;

                for (unsigned int idx = 0; idx < 3; ++idx) 
                {
                    indexBuffer.push_back(face.mIndices[idx] + vertexStartOffset);
                }
            }

            vertexStartOffset += meshPtr->mNumVertices;
        }

        //recursively process children nodes
        for (unsigned int c = 0; c < node->mNumChildren; ++c)
        {
            processModelNode(node->mChildren[c], scene, vertexBuffer, indexBuffer, vertexWriteIndex, vertexStartOffset, currentTransform);
        }
    }

    std::unique_ptr<RBMesh>& RBModel::getMeshes(int index) 
    {
        if(index < 0 || index >= meshes.size()) {
            throw std::out_of_range("Index out of range in RBModel::getMeshes");
        }
        return meshes[index];
    }

    void RBModel::Load(const std::string& path) 
    {
        std::cout << "RBModel::Load() Begin" << std::endl;
        printCurrentWorkingDirectory();
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path.c_str(),
            aiProcess_Triangulate | 
            aiProcess_FlipUVs | 
            aiProcess_GenNormals |
            aiProcess_CalcTangentSpace);

        if (!scene) 
        {
            std::cout << path.c_str() << std::endl;

            std::cerr << "Assimp import failed: " << importer.GetErrorString() << std::endl;
        } 
        else 
        {
            std::cout << "Assimp import succeeded!" << std::endl;
        }

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            throw std::runtime_error("Assimp failed to load model: " + std::string(importer.GetErrorString()));
        }
        std::cout << "Root node name: " << scene->mRootNode->mName.C_Str() << "\n";
        std::cout << "Mesh count in root node: " << scene->mRootNode->mNumMeshes << "\n";
        std::cout << "Child count in root node: " << scene->mRootNode->mNumChildren << "\n";
        
        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        auto mesh = std::make_unique<RBMesh>(path, device, commandBuffer);
        mesh->indexBuffer.data.clear();

        int numTotalVertices = 0;
        for(unsigned int meshItr = 0; meshItr < scene->mNumMeshes; meshItr++)
        {
            numTotalVertices += scene->mMeshes[meshItr]->mNumVertices;
        }
        mesh->vertexBuffer.data.resize(numTotalVertices);
        
        int vertexWriteIndex = 0; 
        int vertexStartOffset = 0; 
        processModelNode(scene->mRootNode, scene, mesh->vertexBuffer.data, mesh->indexBuffer.data, vertexWriteIndex, vertexStartOffset, aiMatrix4x4());

        //create mesh vertex and index buffer
        mesh->InitializeMesh();

        std::cout << "RBModel::Load() Before push_back()!" << std::endl;
        meshes.push_back(std::move(mesh));

        //std::cout << "Mesh vertex count: " << mesh->vertexBuffer.data.size() << std::endl;
        //std::cout << "Mesh index count: " << mesh->indexBuffer.data.size() << std::endl;
        std::cout << "RBModel::loadModel() - model loaded using Assimp" << std::endl;
        std::cout << "RBModel::Load() End" << std::endl;
    }
} // Rottenbamboo
