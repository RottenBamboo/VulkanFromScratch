//
// Created by rottenbamboo on 2025/3/19.
//
#pragma once
#include <glm/gtx/hash.hpp>
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "RBDevice.h"

namespace RottenBamboo {
    class RBBufferManager {

    private:
        RBDevice &rbDevice;

    public:
        VkBufferCreateInfo bufferInfo{};
        VkBuffer buffer{};
        VkMemoryAllocateInfo memoryallocInfo{};
        VkDeviceMemory bufferMemory{};

        RBBufferManager(RBDevice &device);
        ~RBBufferManager();
        void createBuffer();
        void destroyBuffer();
        void fillBufferCreateInfo(VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode);
        void fillMemoryAllocateInfo(VkMemoryPropertyFlags properties);
        void allocateMemory();
        void BindBufferMemory();
        void CreateBufferAllocBindMemory(VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode, VkMemoryPropertyFlags properties);
        void copyMemory(VkDeviceSize size, void *dstData, float*data);
    };
}
