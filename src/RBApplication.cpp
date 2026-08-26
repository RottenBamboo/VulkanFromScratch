//
// Created by rottenbamboo on 2023/5/16.
//

#include "RBApplication.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <thread>

#include "json.h"
#include <fstream>
#include <string>

namespace RottenBamboo {

        RBGUI* RBApplication::ptr_gui = nullptr;
        RBResourceShader* RBApplication::ptr_resourceShader = nullptr;
        std::unordered_map<std::string, RBShaderDefinition>* RBApplication::ptr_shaderDefinition = nullptr;
        std::vector<RBDescriptors*>* RBApplication::ptr_Descriptors = nullptr;
        std::vector<RBDescriptors*> RBApplication::updatedDescriptors{};
        std::vector<RBMaterial>* RBApplication::ptr_Materials = nullptr;
        bool RBApplication::descriptorSetsUpdate = false;
        std::vector<ImageResourcePtr> RBApplication::oldImageResourceVec;
        RBAssetsRegistry* RBApplication::ptr_assetsRegistry;
        
        RBAssetsRegistry* RBApplication::GetAssetRegistry()
        {
            return RBApplication::ptr_assetsRegistry;
        }
        
        RBGUI* RBApplication::GetGUI() 
        {
            return RBApplication::ptr_gui;
        }

        RBResourceShader* RBApplication::GetResourceShader() 
        {
            return RBApplication::ptr_resourceShader;
        }

        std::unordered_map<std::string, RBShaderDefinition>* RBApplication::GetShaderDefinition() 
        {
            return RBApplication::ptr_shaderDefinition;
        }
        RBShaderDefinition* RBApplication::GetShaderDefinition(const std::string path) 
        {
            auto it = ptr_shaderDefinition->find(path);
            return it != ptr_shaderDefinition->end() ? &ptr_shaderDefinition->at(path) : nullptr;
        }
        std::vector<RBDescriptors*>* RBApplication::GetDescriptors()
        {
            return RBApplication::ptr_Descriptors;
        }

        std::vector<RBDescriptors*>* RBApplication::GetUpdatedDescriptors()
        {
            return &RBApplication::updatedDescriptors;
        }

        std::vector<RBMaterial>* RBApplication::GetMaterials()
        {
            return RBApplication::ptr_Materials;
        }
        
        std::vector<ImageResourcePtr>* RBApplication::GetOldImageResource()
        {
            return &oldImageResourceVec;
        }

        RBApplication::RBApplication() 
        {
        uuids::uuid empty;
        assert(empty.is_nil());
        lastFrameTime = std::chrono::high_resolution_clock::now();
        cameraManager = std::make_unique<RBRuntimeCameraManager>();
        InitializeWindow();
        InitializeCamera();
        InitializeDevice();
        InitializeCommandBuffer();
        
        std::string rootPath = GET_PROJECT_ROOT_DIR + "resource";
        assetsRegistry.Initialize(rootPath, cachePath);
        RBApplication::ptr_assetsRegistry = &assetsRegistry;

        model_paths.insert({0, MODEL_PATH});
        model_paths.insert({1, SAMURI_PATH});
        model_paths.insert({2, TERRAIN_PATH});

        InitializeShaderDefinition();
        ptr_shaderDefinition = &shaderDefinitions;
        resourceManager.Load<RBModel>(model_paths);

        for(int i = 0; i < inputShader.size(); i++)
        {
            RBShaderDefinition& rbShaderDef = shaderDefinitions[inputShader[i].shaderDefinitionPath];
            RBShaderDefinitionData* definitionData = &rbShaderDef.GetData();
            std::string pathShader;

            if(inputShader[i].pipelineStage == PipelineStage::PIPELINE_STAGE_VERTEX)
            {
                pathShader = GET_RESOURCE_ROOT_DIR + definitionData->stages[(int)RBShaderStageKind::Vertex].path;
            }
            else if (inputShader[i].pipelineStage == PipelineStage::PIPELINE_STAGE_FRAGMENT)
            {
                pathShader = GET_RESOURCE_ROOT_DIR + definitionData->stages[(int)RBShaderStageKind::Fragment].path;
            }
            
            resourceShader.Load(inputShader[i].stage, pathShader);
            resourceShader.Reflect(std::filesystem::relative(pathShader, GET_RESOURCE_ROOT_DIR).string(), inputShader[i].stage, *resourceShader.Get(inputShader[i].stage));
        }
        RBApplication::ptr_resourceShader = &resourceShader;

        InitializeBuffers();
        
        InitializeMaterial();
        RBApplication::ptr_Materials = &materialsVec;

        updatedDescriptors.clear();
        updatedDescriptors.reserve(0);
        InitializeDescriptors();
        RBApplication::ptr_Descriptors = &m_pDescriptorsGBuffersVec;

        InitializeSwapChain();
        InitializeGraphicPipeline();
        InitializeStaticPtr();

        RBApplication::ptr_gui = &gui;
        InitializeGUI();
        
        InitializeMatrix();
        InitializeEditorMaterial();
        InitializeFileListener();
        
        std::cout << "RBApplication::RBApplication()" << std::endl;
            RBLOG_INFO("RBApplication::RBApplication()");
    }
    void RBApplication::InitializeEditorMaterial()
    {
        editorMaterial.Load(materialsFilePath + "0mech_material" + MAT_EXTENSION);
        RBLOG_INFO("RBApplication::InitializeEditorMaterial()");
    }
    void RBApplication::InitializeMaterial()
    {
        RBMaterial materialLoadItem{"", device, commandBuffer};
        auto count = std::distance(std::filesystem::directory_iterator(GET_RESOURCE_ROOT_DIR + materialsFilePath), std::filesystem::directory_iterator{});
        materialsVec.clear();
        materialsVec.reserve(count);
        for (const auto& entry : std::filesystem::directory_iterator(GET_RESOURCE_ROOT_DIR + materialsFilePath))
        {
            if (entry.is_regular_file() && entry.path().extension() == MAT_EXTENSION)
            {
                std::string relativePath = std::filesystem::relative(entry.path(), GET_RESOURCE_ROOT_DIR).string();
                relativePath = NormalizePathString(relativePath);
                materialLoadItem.Load(relativePath);
                materialsVec.push_back(materialLoadItem);
            }
        }
        RBLOG_INFO("RBApplication::InitializeMaterial()");
    }
    void RBApplication::InitializeShaderDefinition()
    {
        std::vector<std::filesystem::directory_entry> entries;
        // for (const auto& entry : std::filesystem::directory_iterator(GET_RESOURCE_ROOT_DIR + shaderDefinitionFilePath)) 
        // {
        //     if (entry.is_regular_file()) 
        //     {
        //         entries.push_back(entry);
        //     }
        // }
        // std::sort(entries.begin(), entries.end(),[](const std::filesystem::directory_entry& a,const std::filesystem::directory_entry& b) 
        // {
        //     return a.path().filename().string() < b.path().filename().string();
        // });

        //for (const auto& entry : entries) 

        for (const auto& entry : std::filesystem::directory_iterator(GET_RESOURCE_ROOT_DIR + shaderDefinitionFilePath))
        {
            if (entry.is_regular_file() && entry.path().extension() == SHADER_EXTENSION)
            {
                RBShaderDefinition shaderDef;
                std::string relativePath = std::filesystem::relative(entry.path(), GET_RESOURCE_ROOT_DIR).string();
                relativePath = NormalizePathString(relativePath);
                shaderDef.Load(relativePath);
                shaderDefinitions[relativePath] = shaderDef;
            }
        }
    }
    void RBApplication::InitializeStaticPtr()
    {
    }

    RBApplication::~RBApplication() 
    {
        GetAssetRegistry()->SaveCache(cachePath);
    }

    void RBApplication::InitializeWindow()
    {
        windows.InitializeWindow();
        std::cout << "RBApplication::InitializeWindow()" << std::endl;
            RBLOG_INFO("RBApplication::InitializeWindow()");
    }

    void RBApplication::InitializeCamera()
    {
        windows.SetEventCallback([this](const SDL_Event& e) {
            if (cameraManager) {
                cameraManager->OnEvent(e);
            }
        });
        std::cout << "RBApplication::InitializeCamera()" << std::endl;
            RBLOG_INFO("RBApplication::InitializeCamera()");
    }
    void RBApplication::InitializeDevice()
    {
        device.InitializeDevice();
        std::cout << "RBApplication::InitializeDevice()" << std::endl;
            RBLOG_INFO("RBApplication::InitializeDevice()");
    }
    void RBApplication::InitializeCommandBuffer()
    {
        commandBuffer.InitializeCommandBuffer();
        std::cout << "RBApplication::InitializeCommandBuffer()" << std::endl;
            RBLOG_INFO("RBApplication::InitializeCommandBuffer()");
    }

    void RBApplication::InitializeSwapChain()
    {
        swapChain.SetDepthView(&(descriptorsAttachment.rbImageManager.imageBundles[descriptorsAttachment.rbImageManager.getImageCount() - 1].imageView));
        swapChain.InitializeSwapChain();
        std::cout << "RBApplication::InitializeSwapChain()" << std::endl;
            RBLOG_INFO("RBApplication::InitializeSwapChain()");
    }

    void RBApplication::InitializeFileListener()
    {
        m_FileListener = std::make_unique<RBAssetsRegistryListener>(m_FileEventQueue, m_EventMutex);
        m_FileWatcher = std::make_unique<efsw::FileWatcher>();
        
        std::string watchPath = std::filesystem::absolute(GET_RESOURCE_ROOT_DIR).string();
        m_FileWatcher->addWatch(watchPath, m_FileListener.get(), true);
        m_FileWatcher->watch();
    
    RBLOG_INFO("File watcher started on: " + watchPath);
    }

    void RBApplication::InitializeBuffers()
    {
        for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            uniformBuffers[i].CreateBufferNoStageing();
        }

            std::cout << "RBApplication::InitializeBuffers()" << std::endl;
            RBLOG_INFO("RBApplication::InitializeBuffers()");
    }

    void RBApplication::InitializeDescriptors()
    {

        RBSwapChain::SetSwapChainExtent(device, windows);
        m_descriptorsGBuffersVec.reserve(materialsVec.size());
        //GBuffer pass descriptors
        for(const auto& item : materialsVec)
        {
            int descriptorsCount = item.GetData().shaderReflection.descriptorSets.size();
            if(descriptorsCount > 0)
            {
                for(const auto& imageDesc : item.GetData().shaderReflection.descriptorSets)
                {
                    //get image texture
                    int imageCount = imageDesc.second.bindings.size();
                    if(imageDesc.second.bindings.size() > 0)
                    {
                        std::vector<TexturesInfo> texturesInfo;
                        texturesInfo.reserve(imageCount);
                        for(int i = 0; i < imageDesc.second.bindings.size(); i++)
                        {
                            TexturesInfo info;
                            RBShaderParamType type;
                            RBShaderTextureType textureType;
                            //const auto it = imageDesc.second.bindings.find(i + 1);
                            if(imageDesc.second.bindings.find(i + 1) == imageDesc.second.bindings.end())
                            {
                                continue;
                            }
                            const RBDescriptorBinding& imageBindings = imageDesc.second.bindings.find(i + 1)->second;
                            //const RBDescriptorBinding& imageBindings = imageDesc.second.bindings.at(i + 1);
                            if(imageBindings.type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER || imageBindings.type == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE)
                            {
                                //check shaderDefinitiona map type.
                                type = item.GetData().shaderDefinition.GetData().parameters[i].type;
                                textureType = item.GetData().shaderDefinition.GetData().parameters[i].textureType;
                                if(type == RBShaderParamType::Texture2D)
                                {
                                    info.path = imageBindings.texturePath;
                                    switch(textureType)
                                    {
                                        case RBShaderTextureType::Albedo:
                                        {
                                            info.format = VK_FORMAT_R8G8B8A8_SRGB;
                                            info.aspect = VK_IMAGE_ASPECT_COLOR_BIT;
                                            info.isHDR = false;
			                                break;
                                        }
                                        case RBShaderTextureType::Normal:
                                        {
                                            info.format = VK_FORMAT_R8G8B8A8_UNORM;
                                            info.aspect = VK_IMAGE_ASPECT_COLOR_BIT;
                                            info.isHDR = false;
			                                break;
                                        }
                                        default:
                                        {
                                            info.format = VK_FORMAT_R8G8B8A8_SRGB;
                                            info.aspect = VK_IMAGE_ASPECT_COLOR_BIT;
                                            info.isHDR = false;
			                                break;
                                        }
                                    }
                                }
                            }
                            texturesInfo.push_back(info);
                        }
                        //RBDescriptors descriptor{device, commandBuffer};
                        auto descriptor = std::make_unique<RBDescriptors>(device, commandBuffer);
                        descriptor->SetResourcesInfos(uniformBuffers, texturesInfo, false);
                        m_descriptorsGBuffersVec.push_back(std::move(descriptor));
                    }
                }
            }
        }
        
        m_pDescriptorsGBuffersVec.reserve(materialsVec.size());
        for(int i = 0; i < m_descriptorsGBuffersVec.size(); i++)
        {
            m_descriptorsGBuffersVec[i].get()->InitializeDescriptors(resourceShader.GetReflection(RENDER_STAGE_GBUFFER_VERTEX), resourceShader.GetReflection(RENDER_STAGE_GBUFFER_FRAGMENT));
            m_pDescriptorsGBuffersVec.push_back(m_descriptorsGBuffersVec[i].get());
        }

        descriptorsGBuffer.InitializeDescriptors(resourceShader.GetReflection(RENDER_STAGE_GBUFFER_VERTEX), resourceShader.GetReflection(RENDER_STAGE_GBUFFER_FRAGMENT));
        //after GBuffer pass descriptors
        descriptorsSkyBox.InitializeDescriptors(resourceShader.GetReflection(RENDER_STAGE_POST_PROCESSING_VERTEX), resourceShader.GetReflection(RENDER_STAGE_POST_PROCESSING_FRAGMENT));

        descriptorsLighting.InitializeDescriptors(resourceShader.GetReflection(RENDER_STAGE_LIGHTING_VERTEX), resourceShader.GetReflection(RENDER_STAGE_LIGHTING_FRAGMENT), true);
        
        descriptorsAttachment.InitializeDescriptorsFrameBuffer(swapChainExtent, attachmentParams, depthParams, resourceShader.GetReflection(RENDER_STAGE_LIGHTING_VERTEX), resourceShader.GetReflection(RENDER_STAGE_LIGHTING_FRAGMENT), true);
        std::cout << "RBApplication::InitializeDescriptors(const RBResourceShader& resourceShader)" << std::endl;
            RBLOG_INFO("RBApplication::InitializeDescriptors(const RBResourceShader& resourceShader)");
    }

    void RBApplication::InitializeGraphicPipeline()
    {
        gBufferPass.InitializeGraphicPipeline();

        lightPassManager.InitializeGraphicPipeline();

        skyPassManager.InitializeGraphicPipeline();
        std::cout << "RBApplication::InitializeGraphicPipeline()" << std::endl;
            RBLOG_INFO("RBApplication::InitializeGraphicPipeline()");
    };

    void RBApplication::InitializeGUI()
    {
        gui.SetResourceManager(&resourceManager);
        gui.Initialize(swapChain.renderPass);
        gui.SetEditorMaterial(&editorMaterial);
        std::cout << "RBApplication::InitializeGUI()" << std::endl;
            RBLOG_INFO("RBApplication::InitializeGUI()");
    };


    void RBApplication::transformModelVertex(
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

void RBApplication::processModelNode(
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

    void RBApplication::InitializeMatrix()
    {
        // uniformShaderVariables.view = glm::lookAt(glm::vec3(3, 3, 3),
        //                                 glm::vec3(0, 1.3, 0),
        //                                 glm::vec3(0, 1, 0));
        //uniformShaderVariables.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 100.0f);
        //uniformShaderVariables.proj[1][1] *= -1;
        uniformShaderVariables.model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

        uniformShaderVariables.view = mainCamera.GetViewMatrix();
        float aspectRatio = static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
        uniformShaderVariables.proj = mainCamera.GetProjectionMatrix(aspectRatio);
        uniformShaderVariables.screenSize = glm::vec4(swapChainExtent.width, swapChainExtent.height, 1.0f / swapChainExtent.width, 1.0f / swapChainExtent.height);
        uniformShaderVariables.cameraPos = mainCamera.position;
    }

    void RBApplication::updateUniformBuffer(uint32_t currentImage)
    {
        
        //uniformShaderVariables.model = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f));

        uniformShaderVariables.view = mainCamera.GetViewMatrix();
        float aspectRatio = static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
        uniformShaderVariables.proj = mainCamera.GetProjectionMatrix(aspectRatio);
        uniformShaderVariables.screenSize = glm::vec4(swapChainExtent.width, swapChainExtent.height, 1.0f / swapChainExtent.width, 1.0f / swapChainExtent.height);
        uniformShaderVariables.cameraPos = mainCamera.position;
        memcpy(uniformBuffers[currentImage].bufferMapped, &uniformShaderVariables, sizeof(UniformBufferShaderVariables));
    }


    void RBApplication::run() {
        while (!windows.shouldClose()) {

            auto now = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(now - lastFrameTime).count();
            lastFrameTime = now;

            windows.PollEvents();

            auto currentTime = std::chrono::high_resolution_clock::now();
            if (cameraManager) {
                cameraManager->Update(mainCamera, deltaTime);
            }
            drawFrame();
        }
        vkDeviceWaitIdle(device.device);
    }

    void RBApplication::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
        //std::cout << "begin recordCommandBuffer" << std::endl;
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;
        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            RBLOG_FATAL("failed to begin recording command buffer!");
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        std::array<VkClearValue, TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[2].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[3].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[4].depthStencil = {1.0f, 0};

        VkRenderPassBeginInfo gbufferRenderPassInfo{};
        gbufferRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        gbufferRenderPassInfo.renderPass = gBufferPass.renderPass;
        gbufferRenderPassInfo.framebuffer = gBufferPass.gBufferFrameBuffers;
        gbufferRenderPassInfo.renderArea.offset = {0, 0};
        gbufferRenderPassInfo.renderArea.extent = swapChainExtent;
        gbufferRenderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        gbufferRenderPassInfo.pClearValues = clearValues.data();

        
         //std::cout << "before gBufferPass::Execute()" << std::endl;
        // gbuffer pass pipeline
        gBufferPass.Execute(commandBuffer, gbufferRenderPassInfo, m_pDescriptorsGBuffersVec, resourceManager);

        //std::cout << "after gBufferPass::Execute()" << std::endl;

        // VkBuffer lightingVertexBuffers[] = {mesh.vertexBuffer.buffer};
        // VkDeviceSize lightingOffsets[] = {0};

        // for (int i = 0; i < gBufferPass.rbColorAttachmentCount; ++i) {
        //     VkImageMemoryBarrier barrier{};
        //     barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        //     barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        //     barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        //     barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        //     barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        //     barrier.image = descriptorsAttachment.rbImageManager.imageBundles[i].image;
        //     barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        //     barrier.subresourceRange.baseMipLevel = 0;
        //     barrier.subresourceRange.levelCount = 1;
        //     barrier.subresourceRange.baseArrayLayer = 0;
        //     barrier.subresourceRange.layerCount = 1;
        
        //     barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        //     barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        
        //     vkCmdPipelineBarrier(
        //         commandBuffer,
        //         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        //         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        //         0,
        //         0, nullptr,
        //         0, nullptr,
        //         1, &barrier
        //     );
        // }

        VkSampler gbufferSampler = gBufferPass.rbColorAttachmentDescriptors.rbImageManager.imageBundles[0].sampler;
        
        descriptorsLighting.descriptorSetManager.clearDescriptorWrites();

        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = descriptorsLighting.rbBufferPtr[currentFrame].buffer;
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferShaderVariables);

            descriptorsLighting.descriptorSetManager.fillDescriptotSetsWriteBuffer(currentFrame, 0, 0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &bufferInfo);

            //set gbuffer attachment output as lighting pass input
            for(int j = 0; j < gBufferPass.rbColorAttachmentDescriptors.rbImageManager.getImageCount(); j++)
            {
                descriptorsLighting.rbImageManager.imageBundles[j].imageInfo.imageLayout = attachmentParams.layout;
                descriptorsLighting.rbImageManager.imageBundles[j].imageInfo.imageView = gBufferPass.rbColorAttachmentDescriptors.rbImageManager.imageBundles[j].imageView;
                descriptorsLighting.rbImageManager.imageBundles[j].imageInfo.sampler = gBufferPass.rbColorAttachmentDescriptors.rbImageManager.imageBundles[j].sampler;
                descriptorsLighting.descriptorSetManager.fillDescriptotSetsWriteImage(currentFrame, j + 1, 0, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &gBufferPass.rbColorAttachmentDescriptors.rbImageManager.imageBundles[j].imageInfo);
            }

            descriptorsLighting.descriptorSetManager.updateDescriptorSets(device);
        }

        
        // Sky pass: first render on the swap-chain image each frame — clears to black.
        VkRenderPassBeginInfo skyRenderPassInfo{};
        skyRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        skyRenderPassInfo.renderPass = swapChain.renderPassSky;
        skyRenderPassInfo.framebuffer = swapChain.swapChainFrameBuffers[imageIndex];
        skyRenderPassInfo.renderArea.offset = {0, 0};
        skyRenderPassInfo.renderArea.extent = swapChainExtent;

        VkClearValue skyClearValue{};
        skyClearValue.color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        skyRenderPassInfo.clearValueCount = 1;
        skyRenderPassInfo.pClearValues = &skyClearValue;

        // Lighting pass: loads the sky output and composites the deferred lighting result.
        VkRenderPassBeginInfo lightingRenderPassInfo{};
        lightingRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        lightingRenderPassInfo.renderPass = swapChain.renderPass;
        lightingRenderPassInfo.framebuffer = swapChain.swapChainFrameBuffers[imageIndex];
        lightingRenderPassInfo.renderArea.offset = {0, 0};
        lightingRenderPassInfo.renderArea.extent = swapChainExtent;

        VkClearValue lightingClearValue{};
        lightingClearValue.color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        lightingRenderPassInfo.clearValueCount = 1;
        lightingRenderPassInfo.pClearValues = &lightingClearValue;

        skyPassManager.Execute(commandBuffer, skyRenderPassInfo, descriptorsSkyBox);

        lightPassManager.Execute(commandBuffer, lightingRenderPassInfo, descriptorsLighting, gui, uniformShaderVariables);

        //std::cout << "after lightPassManager::Execute()" << std::endl;

        VkResult result = vkEndCommandBuffer(commandBuffer);
        if (result != VK_SUCCESS) {
            std::cerr << "vkEndCommandBuffer failed: " << result << std::endl;
            RBLOG_FATAL("failed to record command buffer!");
            throw std::runtime_error("failed to record command buffer!");
        }
        //std::cout << "RBApplication::Execute()" << std::endl;
    }

    void RBApplication::updateDirtyDescriptorSets()
    {
        if(descriptorSetsUpdate)
        {
            vkDeviceWaitIdle(device.device);

            for(int i = 0; i < updatedDescriptors.size(); i++)
            {
                for(int j = 0; j < updatedDescriptors[i]->imagesInfo.size(); j++)
                {
                    if(updatedDescriptors[i]->imagesInfo[j].needUpdate)
                    {
                        updatedDescriptors[i]->refreshTextureImage(j, updatedDescriptors[i]->imagesInfo[j].path);
                        updatedDescriptors[i]->updateDescriptorSetsTextureImage(j);
                        updatedDescriptors[i]->imagesInfo[j].needUpdate = false;
                    }
                }
            }

            for (int i = 0; i < oldImageResourceVec.size(); i++)
            {
                oldImageResourceVec[i].Reset(&device);                    
            }

            updatedDescriptors.clear();
            oldImageResourceVec.clear();
            descriptorSetsUpdate = false;
            deferredFrameCount = 0;
        }
        //RBLOG_INFO("updateDirtyDescriptorSets");
    }

    void RBApplication::drawFrame()
    {
        //for function 
        processFileEvents();

        vkWaitForFences(device.device, 1, &swapChain.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        vkResetFences(device.device, 1, &swapChain.inFlightFences[currentFrame]);

        //update descriptor sets if needed
        updateDirtyDescriptorSets();

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(device.device, swapChain.swapChain, UINT64_MAX, swapChain.imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            swapChain.recreateSwapChain(&(descriptorsAttachment.rbImageManager.imageBundles[descriptorsAttachment.rbImageManager.getImageCount() - 1].imageView));
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            RBLOG_FATAL("failed to acquire swap chain image!");
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        updateUniformBuffer(currentFrame);

        result = vkResetCommandBuffer(commandBuffer.commandBuffers[currentFrame], /*vkCommandBufferResetFlagBits*/ 0);
        if (result != VK_SUCCESS) {
            RBLOG_FATAL("failed to reset command buffer!");
            throw std::runtime_error("failed to reset command buffer!");
        }

        recordCommandBuffer(commandBuffer.commandBuffers[currentFrame], imageIndex);

        //submit the command buffer
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {swapChain.imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer.commandBuffers[currentFrame];

        VkSemaphore signalSemaphores[] = {swapChain.renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
        result = vkQueueSubmit(device.graphicsQueue, 1, &submitInfo, swapChain.inFlightFences[currentFrame]);
        if (result != VK_SUCCESS)
        {
            std::cerr << "vkQueueSubmit failed with VkResult: " << result << std::endl;
            RBLOG_FATAL("vkQueueSubmit failed with VkResult: %d", result);
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapChain.swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(device.presentQueue, &presentInfo);
        
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || windows.framebufferResized)
        {
            windows.framebufferResized = false;

            vkDeviceWaitIdle(device.device);

            gBufferPass.clearFrameBuffers();

            descriptorsAttachment.ReleaseAllResource();
            RBSwapChain::SetSwapChainExtent(device, windows);
            descriptorsAttachment.InitializeDescriptorsFrameBuffer(swapChainExtent,attachmentParams, depthParams, resourceShader.GetReflection(RENDER_STAGE_LIGHTING_VERTEX), resourceShader.GetReflection(RENDER_STAGE_LIGHTING_FRAGMENT), true);

            gBufferPass.createGraphicsPipeline();

            skyPassManager.createGraphicsPipeline();

            lightPassManager.createGraphicsPipeline();

            swapChain.recreateSwapChain(&(descriptorsAttachment.rbImageManager.imageBundles[descriptorsAttachment.rbImageManager.getImageCount() - 1].imageView));
           
        }
        else if (result != VK_SUCCESS) 
        {
            RBLOG_FATAL("failed to present swap chain image!");
            throw std::runtime_error("failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void RBApplication::processFileEvents()
    {
        std::lock_guard<std::mutex> lock(m_EventMutex);
        while (!m_FileEventQueue.empty()) 
        {
            std::string path = m_FileEventQueue.front();
            m_FileEventQueue.pop();
            std::cout << "[DrawFrame] File event: " << path << std::endl;
        }
    }

}
