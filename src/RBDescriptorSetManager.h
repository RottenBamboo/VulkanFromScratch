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
        void fillDescriptotSet(VkWriteDescriptorSet descriptorWrite);
        void allocateDescriptorSets(RBDevice &device, VkDescriptorSetAllocateInfo &allocInfo, int size);
        void updateDescriptorSets(RBDevice &device, std::vector<VkWriteDescriptorSet> descriptorWrites);
        VkDescriptorSet* getDescriptorSet(int index);
    public:
        std::vector<VkDescriptorSet> descriptorSets;
        std::vector<VkWriteDescriptorSet> descriptorWrites;
    private:
        RBDevice &rbDevice;
    };
}

#endif //VULKANFROMSCRATCH_RBDESCRIPTORSETMANAGER_H
