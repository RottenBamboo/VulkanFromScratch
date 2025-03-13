//
// Created by RottenBamboo on 2023/7/26.
//
//
#include "RBDescriptorSetManager.h"
namespace RottenBamboo{
    RBDescriptorSetManager::RBDescriptorSetManager(RBDevice &device) : rbDevice(device)
    {
    }

    void RBDescriptorSetManager::allocateDescriptorSets(RBDevice &device, VkDescriptorSetAllocateInfo &allocInfo, int size)
    {
        descriptorSets.resize(size);
        if(vkAllocateDescriptorSets(rbDevice.device, &allocInfo, descriptorSets.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }
    }

    VkDescriptorSet* RBDescriptorSetManager::getDescriptorSet(int index)
    {
        return &descriptorSets[index];
    }

    void RBDescriptorSetManager::updateDescriptorSets(RBDevice &device, std::vector<VkWriteDescriptorSet> descriptorWrites)
    {
        vkUpdateDescriptorSets(device.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}