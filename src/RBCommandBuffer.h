//
// Created by rottenbamboo on 2023/6/14.
//

#pragma once
#ifndef VULKANFROMSCRATCH_RBCOMMANDBUFFER_H
#define VULKANFROMSCRATCH_RBCOMMANDBUFFER_H
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/hash.hpp>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include "RBDevice.h"

namespace RottenBamboo {
    class RBCommandBuffer {
    public:
        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;

        void InitializeCommandBuffer();

        RBCommandBuffer(RBDevice &device);

        ~RBCommandBuffer();

        VkCommandBuffer beginSingleTimeCommands(VkCommandBufferLevel level);

        void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    private:
        RBDevice &rbDevice;

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice &device, VkSurfaceKHR *surface);

        void createCommandPool();

        void createCommandBuffers();
    };
}


#endif //VULKANFROMSCRATCH_RBCOMMANDBUFFER_H
