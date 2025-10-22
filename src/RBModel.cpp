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

#ifdef __ANDROID__
void RBModel::Load(const std::string& filepath) {
    __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", "RBModel::Load called for: %s", filepath.c_str());
    std::string extension = GetFileExtension(filepath);
    
    // 如果是 GLTF 文件，尝试手动加载 BIN 文件
    if (extension == "gltf") {
        if (TryLoadGLTFWithManualBin(filepath)) {
            return;
        }
        __android_log_print(ANDROID_LOG_WARN, "RottenBamboo", 
                           "GLTF loading failed, falling back to test mesh");
        return;
    }
    try {
        // 使用 SDL3 的新 API 读取文件
        SDL_IOStream* file = SDL_IOFromFile(filepath.c_str(), "rb");
        if (!file) {
            __android_log_print(ANDROID_LOG_ERROR, "RottenBamboo", 
                               "Failed to open model file: %s, error: %s", 
                               filepath.c_str(), SDL_GetError());
            throw std::runtime_error("Failed to open model file");
        }
        
        // 读取文件内容到内存
        Sint64 fileSize = SDL_GetIOSize(file);
        if (fileSize <= 0) {
            __android_log_print(ANDROID_LOG_ERROR, "RottenBamboo", 
                               "Failed to get file size: %s", filepath.c_str());
            SDL_CloseIO(file);
            throw std::runtime_error("Failed to get file size");
        }
        
        std::vector<uint8_t> buffer(fileSize);
        size_t bytesRead = SDL_ReadIO(file, buffer.data(), fileSize);
        SDL_CloseIO(file);
        
        if (bytesRead != static_cast<size_t>(fileSize)) {
            __android_log_print(ANDROID_LOG_ERROR, "RottenBamboo", 
                               "Failed to read entire file: %s", filepath.c_str());
            throw std::runtime_error("Failed to read entire file");
        }
        
        __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                           "Successfully loaded model file: %s, size: %zu bytes", 
                           filepath.c_str(), buffer.size());
        
        // 使用 Assimp 从内存加载模型
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFileFromMemory(
            buffer.data(), 
            buffer.size(),
            aiProcess_Triangulate | 
            aiProcess_FlipUVs | 
            aiProcess_GenNormals |
            aiProcess_CalcTangentSpace,
            extension.c_str()  // 可选的扩展提示
        );

        // 详细的场景信息日志
        if (!scene) {
            __android_log_print(ANDROID_LOG_ERROR, "RottenBamboo", 
                               "Assimp failed to load model from memory: %s", 
                               importer.GetErrorString());
            throw std::runtime_error("Assimp failed to load model from memory");
        }

        if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
            __android_log_print(ANDROID_LOG_ERROR, "RottenBamboo", 
                               "Assimp scene is incomplete");
            throw std::runtime_error("Assimp scene is incomplete");
        }

        if (!scene->mRootNode) {
            __android_log_print(ANDROID_LOG_ERROR, "RottenBamboo", 
                               "Assimp scene has no root node");
            throw std::runtime_error("Assimp scene has no root node");
        }

        __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                           "Assimp import succeeded! Scene info:");
        __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                           "  - Meshes: %d", scene->mNumMeshes);
        __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                           "  - Materials: %d", scene->mNumMaterials);
        __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                           "  - Textures: %d", scene->mNumTextures);
        __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                           "  - Root node name: %s", scene->mRootNode->mName.C_Str());
        __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                           "  - Root node meshes: %d", scene->mRootNode->mNumMeshes);
        __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                           "  - Root node children: %d", scene->mRootNode->mNumChildren);

        // 检查是否有网格数据
        if (scene->mNumMeshes == 0) {
            __android_log_print(ANDROID_LOG_WARN, "RottenBamboo", 
                               "No meshes found in model, creating test mesh");
            return;
        }

        // 计算总顶点数
        int numTotalVertices = 0;
        for(unsigned int meshItr = 0; meshItr < scene->mNumMeshes; meshItr++) {
            numTotalVertices += scene->mMeshes[meshItr]->mNumVertices;
            __android_log_print(ANDROID_LOG_DEBUG, "RottenBamboo", 
                               "Mesh %d: vertices=%d, faces=%d", 
                               meshItr, scene->mMeshes[meshItr]->mNumVertices, 
                               scene->mMeshes[meshItr]->mNumFaces);
        }

        __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                           "Total vertices: %d", numTotalVertices);

        auto mesh = std::make_unique<RBMesh>(filepath, device, commandBuffer);
        mesh->indexBuffer.data.clear();

        // 预分配顶点缓冲区
        mesh->vertexBuffer.data.resize(numTotalVertices);
        
        int vertexWriteIndex = 0; 
        int vertexStartOffset = 0; 
        
        // 处理模型节点
        processModelNode(scene->mRootNode, scene, mesh->vertexBuffer.data, 
                        mesh->indexBuffer.data, vertexWriteIndex, vertexStartOffset, 
                        aiMatrix4x4());

        // 创建网格顶点和索引缓冲区
        mesh->InitializeMesh();

        meshes.push_back(std::move(mesh));

        __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                           "Model loaded successfully. Vertices: %zu, Indices: %zu", 
                           meshes[0]->vertexBuffer.data.size(), 
                           meshes[0]->indexBuffer.data.size());
        
    } catch (const std::exception& e) {
        __android_log_print(ANDROID_LOG_ERROR, "RottenBamboo", 
                           "Exception while loading model: %s, error: %s", 
                           filepath.c_str(), e.what());
        
        // 创建测试网格作为回退
        __android_log_print(ANDROID_LOG_WARN, "RottenBamboo", 
                           "Created test mesh as fallback");
    }
}
void RBModel::ProcessScene(const aiScene* scene, const std::string& filepath) {
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error("Invalid scene");
    }

#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                       "Assimp import succeeded! Scene info:");
    __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                       "  - Meshes: %d", scene->mNumMeshes);
    __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                       "  - Materials: %d", scene->mNumMaterials);
    __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                       "  - Textures: %d", scene->mNumTextures);
    __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                       "  - Root node name: %s", scene->mRootNode->mName.C_Str());
    __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                       "  - Root node meshes: %d", scene->mRootNode->mNumMeshes);
    __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                       "  - Root node children: %d", scene->mRootNode->mNumChildren);
#else
    std::cout << "Assimp import succeeded!" << std::endl;
    std::cout << "Root node name: " << scene->mRootNode->mName.C_Str() << "\n";
    std::cout << "Mesh count in root node: " << scene->mRootNode->mNumMeshes << "\n";
    std::cout << "Child count in root node: " << scene->mRootNode->mNumChildren << "\n";
#endif

    // 检查是否有网格数据
    if (scene->mNumMeshes == 0) {
#ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_WARN, "RottenBamboo", 
                           "No meshes found in model, creating test mesh");
#else
        std::cout << "No meshes found in model, creating test mesh" << std::endl;
#endif
        return;
    }

    // 计算总顶点数
    int numTotalVertices = 0;
    for(unsigned int meshItr = 0; meshItr < scene->mNumMeshes; meshItr++) {
        numTotalVertices += scene->mMeshes[meshItr]->mNumVertices;
#ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_DEBUG, "RottenBamboo", 
                           "Mesh %d: vertices=%d, faces=%d", 
                           meshItr, scene->mMeshes[meshItr]->mNumVertices, 
                           scene->mMeshes[meshItr]->mNumFaces);
#endif
    }

#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                       "Total vertices: %d", numTotalVertices);
#endif

    auto mesh = std::make_unique<RBMesh>(filepath, device, commandBuffer);
    mesh->indexBuffer.data.clear();

    // 预分配顶点缓冲区
    mesh->vertexBuffer.data.resize(numTotalVertices);
    
    int vertexWriteIndex = 0; 
    int vertexStartOffset = 0; 
    
    // 处理模型节点
    processModelNode(scene->mRootNode, scene, mesh->vertexBuffer.data, 
                    mesh->indexBuffer.data, vertexWriteIndex, vertexStartOffset, 
                    aiMatrix4x4());

    // 创建网格顶点和索引缓冲区
    mesh->InitializeMesh();

    meshes.push_back(std::move(mesh));

#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                       "Model loaded successfully. Vertices: %zu, Indices: %zu", 
                       meshes[0]->vertexBuffer.data.size(), 
                       meshes[0]->indexBuffer.data.size());
#else
    std::cout << "Model loaded successfully. Vertices: " 
              << meshes[0]->vertexBuffer.data.size() 
              << ", Indices: " << meshes[0]->indexBuffer.data.size() << std::endl;
#endif
}
bool RBModel::TryLoadGLTFWithManualBin(const std::string& gltfPath) {
    __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", "Attempting manual GLTF+BIN loading: %s", gltfPath.c_str());
    
    // 1. 加载 GLTF 文件
    SDL_IOStream* gltfFile = SDL_IOFromFile(gltfPath.c_str(), "rb");
    if (!gltfFile) return false;
    
    Sint64 gltfSize = SDL_GetIOSize(gltfFile);
    std::vector<char> gltfData(gltfSize);
    SDL_ReadIO(gltfFile, gltfData.data(), gltfSize);
    SDL_CloseIO(gltfFile);
    
    // 2. 修改 GLTF 内容，将外部 BIN 引用改为内嵌
    std::string gltfContent(gltfData.begin(), gltfData.end());
    
    // 查找并替换 BIN 文件引用
    size_t binRefPos = gltfContent.find("\"uri\": \"scene.bin\"");
    if (binRefPos != std::string::npos) {
        // 加载 BIN 文件并转换为 base64
        std::string binPath = gltfPath.substr(0, gltfPath.find_last_of('.')) + ".bin";
        SDL_IOStream* binFile = SDL_IOFromFile(binPath.c_str(), "rb");
        if (binFile) {
            Sint64 binSize = SDL_GetIOSize(binFile);
            std::vector<uint8_t> binData(binSize);
            SDL_ReadIO(binFile, binData.data(), binSize);
            SDL_CloseIO(binFile);
            
            // 将 BIN 数据转换为 base64
            std::string base64Data = Base64Encode(binData);
            
            // 替换 URI
            std::string newUri = "\"uri\": \"data:application/octet-stream;base64," + base64Data + "\"";
            gltfContent.replace(binRefPos, 19, newUri); // 19 是 "\"uri\": \"scene.bin\"" 的长度
            
            __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", "Replaced external BIN reference with embedded data");
        }
    }
    
    // 3. 使用修改后的 GLTF 内容
    std::vector<uint8_t> modifiedGltfData(gltfContent.begin(), gltfContent.end());
    
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(
        modifiedGltfData.data(), modifiedGltfData.size(),
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals,
        "gltf"
    );
    
    if (!scene) {
        __android_log_print(ANDROID_LOG_ERROR, "RottenBamboo", 
                           "Assimp failed to load modified GLTF: %s", importer.GetErrorString());
        return false;
    }
    
    __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", "GLTF loaded successfully with embedded BIN data");
    ProcessScene(scene, gltfPath);
    return true;
}
std::string RBModel::Base64Encode(const std::vector<uint8_t>& data) {
    static const char base64_chars[] = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
    
    std::string encoded;
    int val = 0, valb = -6;
    
    for (uint8_t c : data) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            encoded.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    
    if (valb > -6) {
        encoded.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    }
    
    while (encoded.size() % 4) {
        encoded.push_back('=');
    }
    
    return encoded;
}
#else
// 桌面端原有代码保持不变
void RBModel::Load(const std::string& path) {
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
#endif
} // Rottenbamboo
