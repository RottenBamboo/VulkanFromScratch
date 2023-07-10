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
#include "RBGraphicPipeline.h"
#include "RBBuffer.h"
#include "RBMesh.h"
#include <stdexcept>
#include <iostream>
#include <string>

namespace RottenBamboo {
    class RBApplication {

    public:
        static const uint32_t WIDTH = 800;
        static const uint32_t HEIGHT = 600;
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

        RBGraphicPipeline graphicPipeline{swapChain, descriptors};
        RBSwapChain swapChain{device, windows, commandBuffer, descriptors};
        RBDescriptors descriptors{device, commandBuffer, uniformBuffers};

        RBMesh mesh{device, commandBuffer};
        RBBuffer<UniformBufferObject> uniformBuffers[MAX_FRAMES_IN_FLIGHT]{{device, commandBuffer, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT}, {device, commandBuffer, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT}};

        RBCommandBuffer commandBuffer{device};
        RBDevice device{windows};
        RBWindows windows{WIDTH, HEIGHT, "Vulkan"};

    private:
        void updateUniformBuffer(uint32_t currentImage);
    };
}

#endif //VULKANFROMSCRATCH_RBAPPLICATION_H
