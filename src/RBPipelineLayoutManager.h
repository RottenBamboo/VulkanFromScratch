//
// Created by rottenbamboo on 2024/7/31.
//

#pragma once
#include <vulkan/vulkan_core.h>
#include "RBDevice.h"

namespace RottenBamboo {

    class RBPipelineLayoutManager {

    private:
        RBDevice &rbDevice;

    public:

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

        VkPipelineLayout pipelineLayout;

        RBPipelineLayoutManager(RBDevice &device);

        ~RBPipelineLayoutManager();

        void fillPipelineLayoutInfo(const VkDescriptorSetLayout *descriptorSetLayout);

        void createPipelineLayout();
    };
}
