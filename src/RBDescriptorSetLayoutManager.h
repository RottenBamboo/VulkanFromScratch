//
// Created by RottenBamboo on 2023/7/26.
//

#pragma once

#include <vulkan/vulkan.h>
#include "RBCommon.h"
#include "RBDevice.h"

namespace RottenBamboo {

    class RBDescriptorSetLayoutManager {

    public:
        VkDescriptorSetLayout descriptorSetLayout{};
    private:
        std::vector<VkDescriptorSetLayoutBinding> bindings{};
        RBDevice &rbDevice;

    public:
        RBDescriptorSetLayoutManager(RBDevice &device);
        void fillDescriptorSetLayoutBinding(uint32_t index, uint32_t descriptorCount, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, const VkSampler* pImmutableSamplers);
        ~RBDescriptorSetLayoutManager();
        void createDescriptorSetLayout();
        void Destroy();
    };

}

