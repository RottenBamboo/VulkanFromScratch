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
        
        RBGUI* RBApplication::GetGUI() {
            return RBApplication::ptr_gui;
        }

        RBResourceShader* RBApplication::GetResourceShader() {
            return RBApplication::ptr_resourceShader;
        }

        RBApplication::RBApplication() {
        lastFrameTime = std::chrono::high_resolution_clock::now();
        cameraManager = std::make_unique<RBRuntimeCameraManager>();
        InitializeWindow();
        InitializeCamera();
        InitializeDevice();
        InitializeCommandBuffer();
        
        model_paths.insert({0, MODEL_PATH});
        model_paths.insert({1, SAMURI_PATH});
        model_paths.insert({2, TERRAIN_PATH});

        resourceManager.Load<RBModel>(model_paths);

        for(int i = 0; i < inputShader.size(); i++)
        {
            resourceShader.Load(inputShader[i].stage, inputShader[i].path);
            resourceShader.Reflect(inputShader[i].path, inputShader[i].stage, *resourceShader.Get(inputShader[i].stage));
        }
        InitializeBuffers();
        InitializeDescriptors();
        InitializeSwapChain();
        InitializeGraphicPipeline();
        InitializeGUI();

        InitializeStaticPtr();
        
        InitializeMatrix();
        InitializeMaterial();
        std::cout << "RBApplication::RBApplication()" << std::endl;
            RBLOG_INFO("RBApplication::RBApplication()");
    }
    void RBApplication::InitializeMaterial()
    {
        editorMaterial.Load(GET_PROJECT_ROOT_DIR + std::string("materials/default_material.mat"));
        std::cout << "RBApplication::InitializeMaterial()" << std::endl;
    }

    void RBApplication::InitializeStaticPtr()
    {
        RBApplication::ptr_gui = &gui;
        RBApplication::ptr_resourceShader = &resourceShader;
    }

    RBApplication::~RBApplication() {
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

        //GBuffer pass descriptors
        descriptorsGBuffersVec.clear();
        descriptorsGBuffersVec.reserve(0);

        descriptorsGBuffer.InitializeDescriptors(resourceShader.GetReflection(SHADER_PIPELINE_STAGE_GBUFFER_VERTEX), resourceShader.GetReflection(SHADER_PIPELINE_STAGE_GBUFFER_FRAGMENT));
        descriptorsGBuffersVec.push_back(&descriptorsGBuffer);

        descriptorsSamuri.InitializeDescriptors(resourceShader.GetReflection(SHADER_PIPELINE_STAGE_GBUFFER_VERTEX), resourceShader.GetReflection(SHADER_PIPELINE_STAGE_GBUFFER_FRAGMENT));
        descriptorsGBuffersVec.push_back(&descriptorsSamuri);

        descriptorsTerrain.InitializeDescriptors(resourceShader.GetReflection(SHADER_PIPELINE_STAGE_GBUFFER_VERTEX), resourceShader.GetReflection(SHADER_PIPELINE_STAGE_GBUFFER_FRAGMENT));
        descriptorsGBuffersVec.push_back(&descriptorsTerrain);

        //after GBuffer pass descriptors
        descriptorsSkyBox.InitializeDescriptors(resourceShader.GetReflection(SHADER_PIPELINE_STAGE_POST_PROCESSING_VERTEX), resourceShader.GetReflection(SHADER_PIPELINE_STAGE_POST_PROCESSING_FRAGMENT));

        descriptorsLighting.InitializeDescriptors(resourceShader.GetReflection(SHADER_PIPELINE_STAGE_LIGHTING_VERTEX), resourceShader.GetReflection(SHADER_PIPELINE_STAGE_LIGHTING_FRAGMENT), true);
        
        descriptorsAttachment.InitializeDescriptorsFrameBuffer(swapChainExtent, attachmentParams, depthParams, resourceShader.GetReflection(SHADER_PIPELINE_STAGE_LIGHTING_VERTEX), resourceShader.GetReflection(SHADER_PIPELINE_STAGE_LIGHTING_FRAGMENT), true);
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
        gBufferPass.Execute(commandBuffer, gbufferRenderPassInfo, descriptorsGBuffersVec, resourceManager);

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

    void RBApplication::drawFrame()
    {
        vkWaitForFences(device.device, 1, &swapChain.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

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

        vkResetFences(device.device, 1, &swapChain.inFlightFences[currentFrame]);

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
            descriptorsAttachment.InitializeDescriptorsFrameBuffer(swapChainExtent,attachmentParams, depthParams, resourceShader.GetReflection(SHADER_PIPELINE_STAGE_LIGHTING_VERTEX), resourceShader.GetReflection(SHADER_PIPELINE_STAGE_LIGHTING_FRAGMENT), true);

            gBufferPass.createGraphicsPipeline();

            skyPassManager.createGraphicsPipeline();

            lightPassManager.createGraphicsPipeline();
            swapChain.recreateSwapChain(&(descriptorsAttachment.rbImageManager.imageBundles[descriptorsAttachment.rbImageManager.getImageCount() - 1].imageView));
        }
        else if (result != VK_SUCCESS) {
            RBLOG_FATAL("failed to present swap chain image!");
            throw std::runtime_error("failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

}
