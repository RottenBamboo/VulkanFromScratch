//
// Created by rottenbamboo on 2025/3/19.
//

#include "RBBufferManager.h"
#include "RBCommon.h"

namespace RottenBamboo
{
    RBBufferManager::RBBufferManager(RBDevice &device) : rbDevice(device)
    {
        std::cout << "RBBufferManager::RBBufferManager()" << std::endl;
        RBLOG_INFO("RBBufferManager::RBBufferManager()");
    }

    RBBufferManager::~RBBufferManager()
    {
        destroyBuffer();
        std::cout << "RBBufferManager::~RBBufferManager()" << std::endl;
        RBLOG_INFO("RBBufferManager::~RBBufferManager()");
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
        RBLOG_INFO("RBBufferManager::fillBufferCreateInfo()");
    }

    void RBBufferManager::createBuffer()
    {
        if (vkCreateBuffer(rbDevice.device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) 
        {
            RBLOG_FATAL("failed to create buffer!");
            throw std::runtime_error("failed to create buffer!");
        }
        std::cout << "RBBufferManager::createBuffer()" << std::endl;
        RBLOG_INFO("RBBufferManager::createBuffer()");
    }

    void RBBufferManager::fillMemoryAllocateInfo(VkMemoryPropertyFlags properties)
    {
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(rbDevice.device, buffer, &memRequirements);

        memoryallocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryallocInfo.allocationSize = memRequirements.size;
        memoryallocInfo.memoryTypeIndex = findMemoryType(rbDevice.physicalDevice, memRequirements.memoryTypeBits, properties);
        std::cout << "RBBufferManager::fillMemoryAllocateInfo()" << std::endl;
        RBLOG_INFO("RBBufferManager::fillMemoryAllocateInfo()");
    }

    void RBBufferManager::allocateMemory()
    {
        if (vkAllocateMemory(rbDevice.device, &memoryallocInfo, nullptr, &bufferMemory) != VK_SUCCESS) 
        {
            RBLOG_FATAL("failed to allocate buffer memory!");
            throw std::runtime_error("failed to allocate buffer memory!");
        }
        std::cout << "RBBufferManager::allocateMemory()" << std::endl;
        RBLOG_INFO("RBBufferManager::allocateMemory()");
    }

    void RBBufferManager::BindBufferMemory()
    {
        if (vkBindBufferMemory(rbDevice.device, buffer, bufferMemory, 0) != VK_SUCCESS) 
        {
            RBLOG_FATAL("failed to bind buffer memory!");
            throw std::runtime_error("failed to bind buffer memory!");
        }
        std::cout << "RBBufferManager::BindBufferMemory()" << std::endl;
        RBLOG_INFO("RBBufferManager::BindBufferMemory()");
    }

    void RBBufferManager::CreateBufferAllocBindMemory(VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode, VkMemoryPropertyFlags properties)
    {
        fillBufferCreateInfo(size, usage, sharingMode);
        createBuffer();
        fillMemoryAllocateInfo(properties);
        allocateMemory();
        BindBufferMemory();
        //RBLOG_INFO("RBBufferManager::CreateBufferAllocBindMemory()");
    }

    void RBBufferManager::copyMemory(VkDeviceSize size, void* dstData, stbi_uc* data)
    {
        vkMapMemory(rbDevice.device, bufferMemory, 0, size, 0, &dstData);
        memcpy(dstData, data, size);
        vkUnmapMemory(rbDevice.device, bufferMemory);
        //RBLOG_INFO("RBBufferManager::copyMemory(VkDeviceSize size, void* dstData, stbi_uc* data)");
    }

    void RBBufferManager::copyMemory(VkDeviceSize size, void* dstData, float* data)
    {
        vkMapMemory(rbDevice.device, bufferMemory, 0, size, 0, &dstData);
        memcpy(dstData, data, size);
        vkUnmapMemory(rbDevice.device, bufferMemory);
        //RBLOG_INFO("RBBufferManager::copyMemory(VkDeviceSize size, void* dstData, float* data)");
    }
}