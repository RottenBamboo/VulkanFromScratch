//
// Created by rottenbamboo on 2023/5/16.
//

#pragma once
#ifndef VULKANFROMSCRATCH_RBAPPLICATION_H
#define VULKANFROMSCRATCH_RBAPPLICATION_H

#include "RBWindows.h"
#include "RBDevice.h"
#include "RBCommandBuffer.h"
#include "RBDescriptors.h"
#include "RBSwapChain.h"
#include "RBGBufferPass.h"
#include "RBLightingPass.h"
#include "RBBuffer.h"
#include "RBMesh.h"
#include <stdexcept>
#include <iostream>
#include <string>

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
        void InitializeDevice();
        void InitializeCommandBuffer();
        void InitializeSwapChain();

        void loadModel();

        void InitializeBuffers();
        void InitializeMesh();
        void InitializeDescriptors();
        void InitializeGraphicPipeline();

        RBWindows windows{WIDTH, HEIGHT, "Vulkan"};
        RBDevice device{windows};
        RBCommandBuffer commandBuffer{device};

        RBMesh mesh{device, commandBuffer};
        RBPipelineConfig pipelineConfig{};
        RBDescriptors descriptors{device, commandBuffer, uniformBuffers};
        RBSwapChain swapChain{device, windows, commandBuffer, descriptors};
        RBGBufferPass gBufferPass{device, swapChain, descriptors, pipelineConfig};
        RBLightingPass lightPassManager{device, swapChain, descriptors, pipelineConfig};

        RBBuffer<UniformBufferObject> uniformBuffers[MAX_FRAMES_IN_FLIGHT]{{device, commandBuffer, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT}, {device, commandBuffer, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT}};


    private:
        void updateUniformBuffer(uint32_t currentImage);
    };
}

#endif //VULKANFROMSCRATCH_RBAPPLICATION_H
