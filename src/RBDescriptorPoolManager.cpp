//
// Created by RottenBamboo on 2023/7/26.
//

#include "RBDescriptorPoolManager.h"

namespace RottenBamboo {

    RBDescriptorPoolManager::RBDescriptorPoolManager(RBDevice &device) : rbDevice(device)
    {
        std::cout << "RBDescriptorPoolManager::RBDescriptorPoolManager()" << std::endl;
            RBLOG_INFO("RBDescriptorPoolManager::RBDescriptorPoolManager()");
    }

    RBDescriptorPoolManager::~RBDescriptorPoolManager()
    {
        Destroy();
        std::cout << "RBDescriptorPoolManager::~RBDescriptorPoolManager()" << std::endl;
            RBLOG_INFO("RBDescriptorPoolManager::~RBDescriptorPoolManager()");
    }

    void RBDescriptorPoolManager::Destroy()
    {
        vkDestroyDescriptorPool(rbDevice.device, descriptorPool, nullptr);
        descriptorPool = VK_NULL_HANDLE;
        poolSizes = {};
        std::cout << "RBDescriptorPoolManager::~Destroy()" << std::endl;
            RBLOG_INFO("RBDescriptorPoolManager::~Destroy()");
    }

    void RBDescriptorPoolManager::fillDescriptorPoolSize(VkDescriptorType type,uint32_t descriptorCount)
    {
        VkDescriptorPoolSize poolSizes{};
        poolSizes.type = type;
        poolSizes.descriptorCount = descriptorCount;
        this->poolSizes.push_back(poolSizes);
        std::cout << "RBDescriptorPoolManager::fillDescriptorPoolSize()" << std::endl;
            RBLOG_INFO("RBDescriptorPoolManager::fillDescriptorPoolSize()");
    }

    void RBDescriptorPoolManager::fillDescriptorPoolCreateInfo(VkStructureType type, uint32_t maxSets)
    {
        poolInfo.sType = type;
        poolInfo.flags = 0;
        poolInfo.pNext = nullptr;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = maxSets;
        std::cout << "RBDescriptorPoolManager::fillDescriptorPoolCreateInfo()" << std::endl;
            RBLOG_INFO("RBDescriptorPoolManager::fillDescriptorPoolCreateInfo()");
    }

    void RBDescriptorPoolManager::CreateDescriptorPool()
    {
        if (vkCreateDescriptorPool(rbDevice.device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            RBLOG_FATAL("failed to create descriptor pool!");
            throw std::runtime_error("failed to create descriptor pool!");
        }
        std::cout << "RBDescriptorPoolManager::CreateDescriptorPool()" << std::endl;
            RBLOG_INFO("RBDescriptorPoolManager::CreateDescriptorPool()");
    }
}
