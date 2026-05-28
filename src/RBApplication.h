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
#include "RBResourceManager.h"
#include "RBResourceShader.h"
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

        void InitializeBuffers();

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

        RBMaterial editorMaterial{GET_PROJECT_ROOT_DIR + std::string("materials/default_material.mat"), device, commandBuffer};

        ResourceManager resourceManager{device, commandBuffer};

        RBResourceShader resourceShader{inputShader[0].path};

        RBPipelineConfig pipelineConfig{};

        std::vector<RBDescriptors*> descriptorsGBuffersVec;

        RBDescriptors descriptorsGBuffer{device, commandBuffer, uniformBuffers, inputImageInfoMech, false};

        RBDescriptors descriptorsTerrain{device, commandBuffer, uniformBuffers, inputImageTerrain, false};

        RBDescriptors descriptorsSamuri{device, commandBuffer, uniformBuffers, samuriTex, false};

        RBDescriptors descriptorsSkyBox{device, commandBuffer, uniformBuffers, inputImageInfoSkyBox, false};

        RBDescriptors descriptorsAttachment{device, commandBuffer, uniformBuffers, true};

        RBDescriptors descriptorsLighting{device, commandBuffer, uniformBuffers, inputImageInfoLighting, true};

        RBSwapChain swapChain{device, windows, commandBuffer};
        
        RBGBufferPass gBufferPass{gBufferPassAttachmentCount, false, true, device, descriptorsGBuffer, descriptorsAttachment, pipelineConfig, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

        RBSkyPass skyPassManager{skyBoxPassColorAttachmentCount, false, false, device, descriptorsSkyBox, pipelineConfig, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

        RBLightingPass lightPassManager{lightPassColorAttachmentCount, true, false, device, descriptorsLighting, pipelineConfig, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

        RBBuffer<UniformBufferShaderVariables> uniformBuffers[MAX_FRAMES_IN_FLIGHT]{{device, commandBuffer, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT}, {device, commandBuffer, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT}};

        UniformBufferShaderVariables uniformShaderVariables{};

    private:

        void updateUniformBuffer(uint32_t currentImage);
        void InitializeStaticPtr();

    public:
        static RBGUI* ptr_gui;

        static RBGUI* GetGUI();

        static RBResourceShader* ptr_resourceShader;

        static RBResourceShader* GetResourceShader();
    };
}
