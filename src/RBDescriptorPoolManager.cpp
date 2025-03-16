//
// Created by RottenBamboo on 2023/7/26.
//

#include "RBDescriptorPoolManager.h"

namespace RottenBamboo {

    RBDescriptorPoolManager::RBDescriptorPoolManager(RBDevice &device) : rbDevice(device)
    {
    }

    RBDescriptorPoolManager::~RBDescriptorPoolManager()
    {
        vkDestroyDescriptorPool(rbDevice.device, descriptorPool, nullptr);
        std::cout << "RBDescriptorPoolManager::~RBDescriptorPoolManager" << std::endl;
    }

    void RBDescriptorPoolManager::fillDescriptorPoolSize(VkDescriptorType type,uint32_t descriptorCount)
    {
        VkDescriptorPoolSize poolSizes{};
        poolSizes.type = type;
        poolSizes.descriptorCount = descriptorCount;
        this->poolSizes.push_back(poolSizes);
    }

    void RBDescriptorPoolManager::fillDescriptorPoolCreateInfo(VkStructureType type, uint32_t maxSets)
    {
        poolInfo.sType = type;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = maxSets;
    }

    void RBDescriptorPoolManager::CreateDescriptorPool()
    {
        if (vkCreateDescriptorPool(rbDevice.device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }
    void RBDescriptorPoolManager::createDescriptorPool() {
//        std::array<VkDescriptorPoolSize, 2> poolSizes{};
//        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//        poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
//
//        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//        poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
//
//        VkDescriptorPoolCreateInfo poolInfo{};
//        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
//        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
//        poolInfo.pPoolSizes = poolSizes.data();
//        poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
//
//        if (vkCreateDescriptorPool(rbDevice.device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
//            throw std::runtime_error("failed to create descriptor pool!");
//        }
    }
}
