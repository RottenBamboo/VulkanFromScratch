//
// Created by rottenbamboo on 2023/6/14.
//

#include "RBCommandBuffer.h"

namespace RottenBamboo {

    QueueFamilyIndices RBCommandBuffer::findQueueFamilies(VkPhysicalDevice &device, VkSurfaceKHR *surface) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto &queueFamily: queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, *surface, &presentSupport);

            if (presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.isComplete()) {
                break;
            }

            i++;
        }
        std::cout << "RBCommandBuffer::findQueueFamilies()" << std::endl;
        return indices;
    }

    RBCommandBuffer::RBCommandBuffer(RBDevice &device) : rbDevice(device) {
        std::cout << "RBCommandBuffer::RBCommandBuffer()" << std::endl;
    }

    void RBCommandBuffer::InitializeCommandBuffer()
    {
        createCommandPool();
        createCommandBuffers();
        std::cout << "RBCommandBuffer::InitializeCommandBuffer()" << std::endl;
    }

    VkCommandBuffer RBCommandBuffer::beginSingleTimeCommands(VkCommandBufferLevel level)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = level;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(rbDevice.device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);
        std::cout << "RBCommandBuffer::beginSingleTimeCommands()" << std::endl;
        return commandBuffer;
    }

    void RBCommandBuffer::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(rbDevice.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(rbDevice.graphicsQueue);

        vkFreeCommandBuffers(rbDevice.device, commandPool, 1, &commandBuffer);
        std::cout << "RBCommandBuffer::endSingleTimeCommands()" << std::endl;
    }

    void RBCommandBuffer::createCommandBuffers() {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

        if (vkAllocateCommandBuffers(rbDevice.device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
        std::cout << "RBCommandBuffer::createCommandBuffers()" << std::endl;
    }

    void RBCommandBuffer::createCommandPool() {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(rbDevice.physicalDevice, &rbDevice.surface);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
        if (vkCreateCommandPool(rbDevice.device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool!");
        }
        std::cout << "RBCommandBuffer::createCommandBuffers()" << std::endl;
    }

    RBCommandBuffer::~RBCommandBuffer() {
        vkDestroyCommandPool(rbDevice.device, commandPool, nullptr);
        std::cout << "RBCommandBuffer::RBCommandBuffer()" << std::endl;
    }
}