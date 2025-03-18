//
// Created by RottenBamboo on 2023/7/26.
//

#pragma once
#ifndef VULKANFROMSCRATCH_RBDESCRIPTORPOOLMANAGER_H
#define VULKANFROMSCRATCH_RBDESCRIPTORPOOLMANAGER_H

#include <vulkan/vulkan.h>
#include <vector>
#include "RBDevice.h"
namespace RottenBamboo {
    class RBDescriptorPoolManager {

    private:
        RBDevice &rbDevice;

        std::vector<VkDescriptorPoolSize> poolSizes{};
        VkDescriptorPoolCreateInfo poolInfo{};

    public:;
        RBDescriptorPoolManager(RBDevice &device);
        ~RBDescriptorPoolManager();
        void fillDescriptorPoolSize(VkDescriptorType type,uint32_t descriptorCount);
        void fillDescriptorPoolCreateInfo(VkStructureType type, uint32_t maxSets);
        void CreateDescriptorPool();
        RBDescriptorPoolManager(const RBDescriptorPoolManager &) = delete;
        RBDescriptorPoolManager(RBDescriptorPoolManager &&) = default;

        VkDescriptorPool descriptorPool{};
    };
}


#endif //VULKANFROMSCRATCH_RBDESCRIPTORPOOLMANAGER_H
