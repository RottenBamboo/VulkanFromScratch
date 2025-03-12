//
// Created by RottenBamboo on 2023/7/26.
//

#pragma once
#ifndef VULKANFROMSCRATCH_RBDESCRIPTORPOOL_H
#define VULKANFROMSCRATCH_RBDESCRIPTORPOOL_H

#include <vulkan/vulkan.h>
#include <vector>
#include "RBDescriptorSetLayoutManager.h"
#include "RBDevice.h"
namespace RottenBamboo {
    class RBDescriptorPool {

        RBDescriptorPool(const RBDescriptorPool &) = delete;
        RBDescriptorPool(RBDescriptorPool &&) = default;
    public:
        void createDescriptorPool();
    private:
        RBDevice &rbDevice;
        const RBDescriptorSetLayoutManager &handle;
        std::vector<VkDescriptorPoolSize> poolSizes;
        std::vector<VkDescriptorPool> pools;
        std::vector<uint32_t> poolSetCount;
    };
}


#endif //VULKANFROMSCRATCH_RBDESCRIPTORPOOL_H
