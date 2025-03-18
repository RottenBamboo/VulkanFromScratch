//
// Created by RottenBamboo on 2023/7/26.
//

#include "RBDescriptorPoolManager.h"

namespace RottenBamboo {

    RBDescriptorPoolManager::RBDescriptorPoolManager(RBDevice &device) : rbDevice(device)
    {
        std::cout << "RBDescriptorPoolManager::RBDescriptorPoolManager()" << std::endl;
    }

    RBDescriptorPoolManager::~RBDescriptorPoolManager()
    {
        vkDestroyDescriptorPool(rbDevice.device, descriptorPool, nullptr);
        std::cout << "RBDescriptorPoolManager::~RBDescriptorPoolManager()" << std::endl;
    }

    void RBDescriptorPoolManager::fillDescriptorPoolSize(VkDescriptorType type,uint32_t descriptorCount)
    {
        VkDescriptorPoolSize poolSizes{};
        poolSizes.type = type;
        poolSizes.descriptorCount = descriptorCount;
        this->poolSizes.push_back(poolSizes);
        std::cout << "RBDescriptorPoolManager::fillDescriptorPoolSize()" << std::endl;
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
    }

    void RBDescriptorPoolManager::CreateDescriptorPool()
    {
        if (vkCreateDescriptorPool(rbDevice.device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
        std::cout << "RBDescriptorPoolManager::CreateDescriptorPool()" << std::endl;
    }
}
