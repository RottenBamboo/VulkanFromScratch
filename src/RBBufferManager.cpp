//
// Created by 王旭 on 2025/3/19.
//

#include "RBBufferManager.h"
#include "RBCommon.h"

namespace RottenBamboo
{
    RBBufferManager::RBBufferManager(RBDevice &device) : rbDevice(device)
    {
        std::cout << "RBBufferManager::RBBufferManager()" << std::endl;
    }

    RBBufferManager::~RBBufferManager()
    {
        DestroyBuffer();
        std::cout << "RBBufferManager::~RBBufferManager()" << std::endl;
    }

    void RBBufferManager::destroyBuffer()
    {
        vkDestroyBuffer(rbDevice.device, buffer, nullptr);
        vkFreeMemory(rbDevice.device, bufferMemory, nullptr);
    }

    void RBBufferManager::fillBufferCreateInfo(VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode)
    {
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = sharingMode;
        std::cout << "RBBufferManager::fillBufferCreateInfo()" << std::endl;
    }

    void RBBufferManager::createBuffer()
    {
        if (vkCreateBuffer(rbDevice.device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
        }
        std::cout << "RBBufferManager::createBuffer()" << std::endl;
    }

    void RBBufferManager::fillMemoryAllocateInfo(VkMemoryPropertyFlags properties)
    {
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(rbDevice.device, buffer, &memRequirements);

        memoryallocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryallocInfo.allocationSize = memRequirements.size;
        memoryallocInfo.memoryTypeIndex = findMemoryType(rbDevice.physicalDevice, memRequirements.memoryTypeBits, properties);
        std::cout << "RBBufferManager::fillMemoryAllocateInfo()" << std::endl;
    }

    void RBBufferManager::allocateMemory()
    {
        if (vkAllocateMemory(rbDevice.device, &memoryallocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }
        std::cout << "RBBufferManager::allocateMemory()" << std::endl;
    }

    void RBBufferManager::BindBufferMemory()
    {
        if (vkBindBufferMemory(rbDevice.device, buffer, bufferMemory, 0) != VK_SUCCESS) {
            throw std::runtime_error("failed to bind buffer memory!");
        }
        std::cout << "RBBufferManager::BindBufferMemory()" << std::endl;
    }

    void RBBufferManager::CreateBufferAllocBindMemory(VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode, VkMemoryPropertyFlags properties)
    {
        fillBufferCreateInfo(size, usage, sharingMode);
        createBuffer();
        fillMemoryAllocateInfo(properties);
        allocateMemory();
        BindBufferMemory();
    }

    void RBBufferManager::copyMemory(VkDeviceSize size, void *data, unsigned char *dstData)
    {
        vkMapMemory(rbDevice.device, bufferMemory, 0, size, 0, &data);
        memcpy(data, dstData, size);
        vkUnmapMemory(rbDevice.device, bufferMemory);
    }
}