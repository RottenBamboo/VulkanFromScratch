//
// Created by RottenBamboo on 2023/7/26.
//

#pragma once

#ifndef VULKANFROMSCRATCH_RBDESCRIPTORSETMANAGER_H
#define VULKANFROMSCRATCH_RBDESCRIPTORSETMANAGER_H


#include <vulkan/vulkan.h>
#include <vector>
#include "RBDevice.h"
#include "RBDescriptorSetLayoutManager.h"

namespace RottenBamboo {
    class RBDescriptorSetManager {

    private:
        RBDevice &rbDevice;

    public:
        RBDescriptorSetLayoutManager descriptorSetLayoutManager{rbDevice};
        VkDescriptorSetAllocateInfo allocInfo;
        std::vector<VkDescriptorSet> descriptorSets;
        std::vector<VkWriteDescriptorSet> descriptorWrites;

    public:
        RBDescriptorSetManager(RBDevice &device);
        ~RBDescriptorSetManager();
        void fillDescriptorSetsAllocateInfo(VkDescriptorPool &descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSetLayout* pSetLayouts);
        void fillDescriptotSetsWriteBuffer(uint32_t dstSetIndex, uint32_t dstBinding, uint32_t dstArrayElement, uint32_t descriptorCount, VkDescriptorType descriptorType, const VkDescriptorBufferInfo* pBufferInfo);
        void fillDescriptotSetsWriteImage(uint32_t dstSetIndex, uint32_t dstBinding, uint32_t dstArrayElement, uint32_t descriptorCount, VkDescriptorType descriptorType, const VkDescriptorImageInfo* pImageInfo);
        void allocateDescriptorSets(RBDevice &device, int size);
        void updateDescriptorSets(RBDevice &device);
        void updateDescriptorSets(RBDevice &device, std::vector<VkWriteDescriptorSet> descriptorWrites);
        void clearDescriptorWrites();
        void clearDescriptorSets();
        void Destroy();
        VkDescriptorSet* getDescriptorSet(int index);
    };
}

#endif //VULKANFROMSCRATCH_RBDESCRIPTORSETMANAGER_H
