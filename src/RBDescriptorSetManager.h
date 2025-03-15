//
// Created by RottenBamboo on 2023/7/26.
//

#ifndef VULKANFROMSCRATCH_RBDESCRIPTORSETMANAGER_H
#define VULKANFROMSCRATCH_RBDESCRIPTORSETMANAGER_H


#include <vulkan/vulkan.h>
#include <vector>
#include "RBDevice.h"

namespace RottenBamboo {
    class RBDescriptorSetManager {

    public:
        RBDescriptorSetManager(RBDevice &device);
        ~RBDescriptorSetManager();
        void allocateDescriptorSets(RBDevice &device, VkDescriptorSetAllocateInfo &allocInfo, int size);
        void updateDescriptorSets(RBDevice &device);
        void updateDescriptorSets(RBDevice &device, std::vector<VkWriteDescriptorSet> descriptorWrites);
        void fillDescriptotSetsWriteBuffer(uint32_t dstSetIndex, uint32_t dstBinding, uint32_t dstArrayElement, uint32_t descriptorCount, VkDescriptorType descriptorType, const VkDescriptorBufferInfo* pBufferInfo);
        void fillDescriptotSetsWriteImage(uint32_t dstSetIndex, uint32_t dstBinding, uint32_t dstArrayElement, uint32_t descriptorCount, VkDescriptorType descriptorType, const VkDescriptorImageInfo* pImageInfo);
        void clearDescriptorWrites();
        void clearDescriptorSets();
        VkDescriptorSet* getDescriptorSet(int index);
    public:
        std::vector<VkDescriptorSet> descriptorSets;
        std::vector<VkWriteDescriptorSet> descriptorWrites;
    private:
        RBDevice &rbDevice;
    };
}

#endif //VULKANFROMSCRATCH_RBDESCRIPTORSETMANAGER_H
