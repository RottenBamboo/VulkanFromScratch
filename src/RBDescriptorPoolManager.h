//
// Created by RottenBamboo on 2023/7/26.
//

#pragma once
#ifndef VULKANFROMSCRATCH_RBDESCRIPTORPOOLMANAGER_H
#define VULKANFROMSCRATCH_RBDESCRIPTORPOOLMANAGER_H

#include <vulkan/vulkan.h>
#include <vector>
#include "RBDescriptorSetLayoutManager.h"
#include "RBDevice.h"
namespace RottenBamboo {
    class RBDescriptorPoolManager {

    private:
        RBDevice &rbDevice;
    public:;
        void fillDescriptorPoolSize(VkDescriptorType type,uint32_t descriptorCount);
        void fillDescriptorPoolCreateInfo(VkStructureType type, uint32_t maxSets);
        void CreateDescriptorPool();
        RBDescriptorPoolManager(RBDevice &device);
        ~RBDescriptorPoolManager();
        void createDescriptorPool();
        RBDescriptorPoolManager(const RBDescriptorPoolManager &) = delete;
        RBDescriptorPoolManager(RBDescriptorPoolManager &&) = default;
        //const RBDescriptorSetLayoutManager &handle;
        std::vector<VkDescriptorPoolSize> poolSizes;
        std::vector<VkDescriptorPool> pools;
        std::vector<uint32_t> poolSetCount;
        VkDescriptorPoolCreateInfo poolInfo;
        VkDescriptorPool descriptorPool;
    };
}


#endif //VULKANFROMSCRATCH_RBDESCRIPTORPOOLMANAGER_H
