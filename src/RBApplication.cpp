//
// Created by rottenbamboo on 2023/5/16.
//

#include "RBApplication.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <chrono>
#include <thread>

namespace RottenBamboo {

    RBApplication::RBApplication() {
        InitializeWindow();
        InitializeDevice();
        InitializeCommandBuffer();
        loadModel();
        InitializeBuffers();
        InitializeDescriptors();
        InitializeSwapChain();
        InitializeGraphicPipeline();
        std::cout << "RBApplication::RBApplication()" << std::endl;
    }

    RBApplication::~RBApplication() {
    }

    void RBApplication::InitializeWindow()
    {
        windows.InitializeWindow();
        std::cout << "RBApplication::InitializeWindow()" << std::endl;
    }

    void RBApplication::InitializeDevice()
    {
        device.InitializeDevice();
        std::cout << "RBApplication::InitializeDevice()" << std::endl;
    }
    void RBApplication::InitializeCommandBuffer()
    {
        commandBuffer.InitializeCommandBuffer();
        std::cout << "RBApplication::InitializeCommandBuffer()" << std::endl;
    }

    void RBApplication::InitializeSwapChain()
    {
        swapChain.InitializeSwapChain();
        std::cout << "RBApplication::InitializeSwapChain()" << std::endl;
    }

    void RBApplication::InitializeBuffers()
    {
        for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            uniformBuffers[i].CreateBufferNoStageing();
        }

        InitializeMesh();
        std::cout << "RBApplication::InitializeBuffers()" << std::endl;
    }

    void RBApplication::InitializeMesh()
    {
        mesh.InitializeMesh();
        std::cout << "RBApplication::InitializeMesh()" << std::endl;
    }

    void RBApplication::InitializeDescriptors()
    {
        descriptorsGBuffer.InitializeDescriptors();

        descriptors.InitializeDescriptors();

        descriptorsLighting.InitializeDescriptors();
        std::cout << "RBApplication::InitializeDescriptors()" << std::endl;
    }

    void RBApplication::InitializeGraphicPipeline()
    {
        gBufferPass.InitializeGraphicPipeline();

        lightPassManager.InitializeGraphicPipeline();
        std::cout << "RBApplication::InitializeGraphicPipeline()" << std::endl;
    };

    void RBApplication::loadModel()
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str()))
        {
            throw std::runtime_error(warn + err);
        }

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        for(const auto& shape : shapes)
        {
            for(const auto& index : shape.mesh.indices)
            {
                Vertex vertex{};

                vertex.pos = {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = {1.0f, 1.0f, 1.0f};

                if(uniqueVertices.count(vertex) == 0)
                {
                    uniqueVertices[vertex] = static_cast<uint32_t>(mesh.vertexBuffer.data.size());
                    mesh.vertexBuffer.data.push_back(vertex);
                }

                mesh.indexBuffer.data.push_back(uniqueVertices[vertex]);
            }
        }
        std::cout << "RBApplication::loadModel()" << std::endl;
    }

    void RBApplication::updateUniformBuffer(uint32_t currentImage)
    {
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        ubo.model = glm::scale(ubo.model, glm::vec3(0.1f, 0.1f, 0.1f));
        ubo.view = glm::lookAt(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float) swapChainExtent.height, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;

        memcpy(uniformBuffers[currentImage].bufferMapped, &ubo, sizeof(ubo));
    }

    void RBApplication::run() {
        while (!windows.shouldClose()) {
            glfwPollEvents();
            drawFrame();
            float interval = C_intervalTime - (glfwGetTime() - timeStamp);
            //int intervalMiliseconds = int(interval * 1000);
            //std::this_thread::sleep_for(std::chrono::milliseconds(intervalMiliseconds));
            timeStamp = glfwGetTime();
        }
        vkDeviceWaitIdle(device.device);
    }

    void RBApplication::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
        std::cout << "begin recordCommandBuffer" << std::endl;
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;
        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        std::array<VkClearValue, 4> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[2].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[3].color = {{0.0f, 0.0f, 0.0f, 1.0f}};

        VkRenderPassBeginInfo gbufferRenderPassInfo{};
        gbufferRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        gbufferRenderPassInfo.renderPass = gBufferPass.renderPass;
        gbufferRenderPassInfo.framebuffer = gBufferPass.gBufferFrameBuffers;
        gbufferRenderPassInfo.renderArea.offset = {0, 0};
        gbufferRenderPassInfo.renderArea.extent = swapChainExtent;
        gbufferRenderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        gbufferRenderPassInfo.pClearValues = clearValues.data();

        VkBuffer vertexBuffers[] = {mesh.vertexBuffer.buffer};
        VkDeviceSize offsets[] = {0};
        
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffer, mesh.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
        
        std::cout << "before gBufferPass::recordCommandBuffer()" << std::endl;
        // gbuffer pass pipeline
        gBufferPass.recordCommandBuffer(commandBuffer, imageIndex, gbufferRenderPassInfo, descriptorsGBuffer, mesh);
        //std::cout << "after gBufferPass::recordCommandBuffer()" << std::endl;

        // VkBuffer lightingVertexBuffers[] = {mesh.vertexBuffer.buffer};
        // VkDeviceSize lightingOffsets[] = {0};

        for (int i = 0; i < gBufferPass.rbColorAttachmentCount; ++i) {
            VkImageMemoryBarrier barrier{};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image = descriptorsLighting.rbImageManager.imageBundles[i].image;
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.levelCount = 1;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;
        
            barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        
            vkCmdPipelineBarrier(
                commandBuffer,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                0,
                0, nullptr,
                0, nullptr,
                1, &barrier
            );
        }

        VkSampler gbufferSampler = gBufferPass.rbColorAttachmentDescriptors.rbImageManager.imageBundles[0].sampler;
        
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorsLighting.descriptorSetManager.descriptorSetLayoutManager.descriptorSetLayout);
        std::cout << "gBuffer To Lighting" << layouts.size() << std::endl;
        //descriptorsLighting.descriptorSetManager.fillDescriptorSetsAllocateInfo(descriptorsLighting.descriptorSetManager.descriptorPoolManager.descriptorPool, MAX_FRAMES_IN_FLIGHT, layouts.data());
        descriptorsLighting.descriptorSetManager.allocateDescriptorSets(device, MAX_FRAMES_IN_FLIGHT);


        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = descriptorsLighting.rbBufferPtr[imageIndex].buffer;
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            descriptorsLighting.descriptorSetManager.fillDescriptotSetsWriteBuffer(imageIndex, 0, 0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &bufferInfo);

            for(int j = 0; j < 4; j++)
            {
                descriptorsLighting.rbImageManager.imageBundles[j].imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                descriptorsLighting.rbImageManager.imageBundles[j].imageInfo.imageView = gBufferPass.rbColorAttachmentDescriptors.rbImageManager.imageBundles[j].imageView;
                descriptorsLighting.rbImageManager.imageBundles[j].imageInfo.sampler = gBufferPass.rbColorAttachmentDescriptors.rbImageManager.imageBundles[j].sampler;
                descriptorsLighting.descriptorSetManager.fillDescriptotSetsWriteImage(imageIndex, j + 1, 0, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &gBufferPass.rbColorAttachmentDescriptors.rbImageManager.imageBundles[j].imageInfo);
            }

            descriptorsLighting.descriptorSetManager.updateDescriptorSets(device);
        }

        
        VkRenderPassBeginInfo lightingRenderPassInfo{};
        lightingRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        lightingRenderPassInfo.renderPass = swapChain.renderPass;
        lightingRenderPassInfo.framebuffer = swapChain.swapChainFrameBuffers[imageIndex];
        lightingRenderPassInfo.renderArea.offset = {0, 0};
        lightingRenderPassInfo.renderArea.extent = swapChainExtent;

        std::array<VkClearValue, 2> lightingClearValues{};
        lightingClearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        lightingClearValues[1].depthStencil = {1.0f, 0};

        lightingRenderPassInfo.clearValueCount = static_cast<uint32_t>(lightingClearValues.size());
        lightingRenderPassInfo.pClearValues = lightingClearValues.data();

        //lighting pass pipeline
        std::cout << "before lightPassManager::recordCommandBuffer()" << std::endl;
        std::cout << "descriptorsLighting.rbImageManager.imageBundles[0].imageInfo.imageLayout = " << descriptorsLighting.rbImageManager.imageBundles[0].imageInfo.imageLayout << std::endl;
        lightPassManager.recordCommandBuffer(commandBuffer, imageIndex, lightingRenderPassInfo, descriptorsLighting, mesh);

        //std::cout << "after lightPassManager::recordCommandBuffer()" << std::endl;
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
        //std::cout << "RBApplication::recordCommandBuffer()" << std::endl;
    }

    void RBApplication::drawFrame()
    {
        vkWaitForFences(device.device, 1, &swapChain.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(device.device, swapChain.swapChain, UINT64_MAX, swapChain.imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            swapChain.recreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        updateUniformBuffer(currentFrame);

        vkResetFences(device.device, 1, &swapChain.inFlightFences[currentFrame]);

        result = vkResetCommandBuffer(commandBuffer.commandBuffers[currentFrame], /*vkCommandBufferResetFlagBits*/ 0);
        if (result != VK_SUCCESS) {
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
        if (vkQueueSubmit(device.graphicsQueue, 1, &submitInfo, swapChain.inFlightFences[currentFrame]) != VK_SUCCESS)
        {
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
            swapChain.recreateSwapChain();
            
            gBufferPass.createGraphicsPipeline();

            lightPassManager.createGraphicsPipeline();
        }
        else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

}