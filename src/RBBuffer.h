//
// Created by rottembamboo on 2023/6/25.
//

#pragma once

#include "RBCommon.h"
#include "RBCommandBuffer.h"
#include <vector>

namespace RottenBamboo {
    template<typename T>
    class RBBuffer {
    private:
        RBDevice &rbDevice;
        RBCommandBuffer &rbCommandBuffer;
    public:

        VkBuffer buffer;
        VkDeviceMemory memory;
        std::vector<T> data;
        void* bufferMapped;
        VkBufferUsageFlagBits rbFlagBits;
        ~RBBuffer()
        {
            ReleaseBuffer();
        }

        RBBuffer(RBDevice& device, RBCommandBuffer& commandBuffer, VkBufferUsageFlagBits flagBits) : rbDevice(device), rbCommandBuffer(commandBuffer), rbFlagBits(flagBits)
        {
        }

        void ReleaseBuffer()
        {
            vkDestroyBuffer(rbDevice.device, buffer, nullptr);
            vkFreeMemory(rbDevice.device, memory, nullptr);
            data.clear();
            bufferMapped = nullptr;
            std::cout << "RBBuffer::ReleaseBuffer()" << std::endl;
        }

        void CreateBuffer()
        {
            VkDeviceSize bufferSize = sizeof(data[0]) * data.size();
            VkBuffer stagingBuffer;
            VkDeviceMemory stagingBufferMemory;
            createBufferMemory(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

            void* pBufferData;
            vkMapMemory(rbDevice.device, stagingBufferMemory, 0, bufferSize, 0, &pBufferData);
            memcpy(pBufferData, data.data(), (size_t) bufferSize);
            vkUnmapMemory(rbDevice.device, stagingBufferMemory);

            createBufferMemory(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | rbFlagBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, memory);

            copyBuffer(stagingBuffer, buffer, bufferSize);

            vkDestroyBuffer(rbDevice.device, stagingBuffer, nullptr);
            vkFreeMemory(rbDevice.device, stagingBufferMemory, nullptr);
            std::cout << "RBBuffer::CreateBuffer()" << std::endl;
        }

        void CreateBufferNoStageing()
        {
            VkDeviceSize bufferSize = sizeof(T);
            data.resize(1);
            bufferMapped = data.data();
            createBufferMemory(bufferSize, rbFlagBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, buffer, memory);

            vkMapMemory(rbDevice.device, memory, 0, bufferSize, 0, &bufferMapped);
            std::cout << "RBBuffer::CreateBufferNoStageing()" << std::endl;
        }

    private:
        VkCommandBuffer beginSingleTimeCommands()
        {
            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandPool = rbCommandBuffer.commandPool;
            allocInfo.commandBufferCount = 1;
            VkCommandBuffer commandBuffer;
            vkAllocateCommandBuffers(rbDevice.device, &allocInfo, &commandBuffer);

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(commandBuffer, &beginInfo);
            return commandBuffer;
            std::cout << "RBBuffer::beginSingleTimeCommands()" << std::endl;
        }

        void endSingleTimeCommands(VkCommandBuffer commandBuffer) {
            vkEndCommandBuffer(commandBuffer);

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            vkQueueSubmit(rbDevice.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(rbDevice.graphicsQueue);

            vkFreeCommandBuffers(rbDevice.device, rbCommandBuffer.commandPool, 1, &commandBuffer);
            std::cout << "RBBuffer::endSingleTimeCommands()" << std::endl;
        }

        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
            VkCommandBuffer commandBuffer = beginSingleTimeCommands();

            VkBufferCopy copyRegion{};
            copyRegion.size = size;

            vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

            endSingleTimeCommands(commandBuffer);
            std::cout << "RBBuffer::copyBuffer()" << std::endl;
        }

        void createBufferMemory(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = size;
            bufferInfo.usage = usage;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (vkCreateBuffer(rbDevice.device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
                throw std::runtime_error("failed to create buffer!");
            }

            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(rbDevice.device, buffer, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, rbDevice.physicalDevice);

            if (vkAllocateMemory(rbDevice.device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
                throw std::runtime_error("failed to allocate buffer memory!");
            }

            vkBindBufferMemory(rbDevice.device, buffer, bufferMemory, 0);
            std::cout << "RBBuffer::createBufferMemory()" << std::endl;
        }

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice& physicalDevice)
        {
            VkPhysicalDeviceMemoryProperties memProperties;
            vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
            for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
                if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                    return i;
                }
            }

            //throw std::runtime_error("failed to find suitable memory type!");
            std::cout << "RBBuffer::findMemoryType()" << std::endl;
        }
    };

}
