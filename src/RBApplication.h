//
// Created by rottenbamboo on 2023/5/16.
//

#pragma once

#include "RBWindows.h"
#include "RBDevice.h"
#include "RBCommandBuffer.h"
#include "RBSwapChain.h"
#include "RBGBufferPass.h"
#include "RBSkyPass.h"
#include "RBLightingPass.h"
#include "RBBuffer.h"
#include "RBMesh.h"
#include "RBRuntimeCameraManager.h"
#include "RBEditorCameraManager.h"
#include <stdexcept>
#include <iostream>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <chrono>

namespace RottenBamboo {
    #define attachmentUsageFlagBits (VkImageUsageFlagBits)(VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
    class RBApplication {

    public:
        static const uint32_t WIDTH = 1920;
        static const uint32_t HEIGHT = 1080;

        void run();

        void drawFrame();

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        RBApplication();

        ~RBApplication();

        void InitializeWindow();

        void InitializeCamera();

        void InitializeDevice();

        void InitializeCommandBuffer();

        void InitializeSwapChain();

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

        void loadModelAssimp();

        void loadModel();

        void InitializeBuffers();

        void InitializeMesh();

        void InitializeDescriptors();

        void InitializeGraphicPipeline();

        void InitializeGUI();

        void InitializeMatrix();

        std::chrono::high_resolution_clock::time_point lastFrameTime;

        RBWindows windows{WIDTH, HEIGHT, "Vulkan"};

        RBCamera mainCamera;

        std::unique_ptr<RBCameraManager> cameraManager;

        RBDevice device{windows};

        RBGUI gui{device, windows};

        RBCommandBuffer commandBuffer{device};

        RBMesh mesh{device, commandBuffer};

        RBPipelineConfig pipelineConfig{};
        
        std::array<VkImageUsageFlagBits, TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT> lightingImageUsageFlags{attachmentUsageFlagBits, attachmentUsageFlagBits, attachmentUsageFlagBits, attachmentUsageFlagBits, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT};

        std::array<VkImageAspectFlagBits, TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT> lightingImageAspectFlagBits{VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_ASPECT_DEPTH_BIT};

        std::array<VkFormat, TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT> lightingImageFormats{VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_D32_SFLOAT};

        std::array<VkImageLayout, TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT> attahmentLayouts{VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL};

        std::array<VkImageLayout, TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT> gbufferLayouts{VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL};
        
        RBDescriptors<TEXTURE_PATHS_COUNT, 1> descriptors{device, commandBuffer, uniformBuffers, inputImagesInfo, false};

        RBDescriptors<TEXTURE_PATHS_MECH_COUNT, 1> descriptorsGBuffer{device, commandBuffer, uniformBuffers, inputImageInfoMech, false};

        RBDescriptors<TEXTURE_PATHS_SKYBOX_COUNT, 1> descriptorsSkyBox{device, commandBuffer, uniformBuffers, inputImageInfoSkyBox, false};

        RBDescriptors<TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT, 1> descriptorsAttachment{device, commandBuffer, uniformBuffers, true};

        RBDescriptors<TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT, 1> descriptorsLighting{device, commandBuffer, uniformBuffers, inputImageInfoLighting, true};

        RBSwapChain swapChain{device, windows, commandBuffer, descriptors};
        
        RBGBufferPass gBufferPass{gBufferPassAttachmentCount, false, true, device, descriptorsGBuffer, descriptorsAttachment, pipelineConfig, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

        RBSkyPass skyPassManager{skyBoxPassColorAttachmentCount, false, false, device, descriptorsSkyBox, pipelineConfig, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

        RBLightingPass lightPassManager{lightPassColorAttachmentCount, true, false, device, descriptorsLighting, pipelineConfig, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

        RBBuffer<UniformBufferShaderVariables> uniformBuffers[MAX_FRAMES_IN_FLIGHT]{{device, commandBuffer, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT}, {device, commandBuffer, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT}};

        UniformBufferShaderVariables uniformShaderVariables{};

    private:

        void updateUniformBuffer(uint32_t currentImage);
    };
}
