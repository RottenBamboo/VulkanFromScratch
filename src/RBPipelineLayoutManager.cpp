//
// Created by rottenbamboo on 2024/7/31.
//

#include "RBPipelineLayoutManager.h"

namespace RottenBamboo {
    RBPipelineLayoutManager::RBPipelineLayoutManager(RBDevice &device) : rbDevice(device) {
        std::cout << "RBPipelineLayoutManager::RBPipelineLayoutManager()" << std::endl;
    }

    void RBPipelineLayoutManager::fillPipelineLayoutInfo(const VkDescriptorSetLayout *descriptorSetLayout)
    {
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayout;
        pipelineLayoutInfo.pNext = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        pipelineLayoutInfo.flags = 0;
        std::cout << "RBPipelineLayoutManager::fillPipelineLayoutInfo()" << std::endl;
    }

    void RBPipelineLayoutManager::createPipelineLayout()
    {
        if (vkCreatePipelineLayout(rbDevice.device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw ::std::runtime_error("failed to create pipeline layout");
        }
        std::cout << "RBPipelineLayoutManager::createPipelineLayout()" << std::endl;
    }

    RBPipelineLayoutManager::~RBPipelineLayoutManager()
    {
        vkDestroyPipelineLayout(rbDevice.device, pipelineLayout, nullptr);
        std::cout << "RBPipelineLayoutManager::~RBPipelineLayoutManager()" << std::endl;
    }
}