//
// Created by rottenbamboo on 2024/7/31.
//

#ifndef VULKANFROMSCRATCH_RBPIPELINELAYOUTMANAGER_H
#define VULKANFROMSCRATCH_RBPIPELINELAYOUTMANAGER_H


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


#endif //VULKANFROMSCRATCH_RBPIPELINELAYOUTMANAGER_H
