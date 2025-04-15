//
// Created by rottenbamboo on 2023/5/22.
//

#pragma once
//#ifndef VULKANFROMSCRATCH_RBGRAPHICPIPELINEMANAGER_H
//#define VULKANFROMSCRATCH_RBGRAPHICPIPELINEMANAGER_H

#include <fstream>
#include <tiny_obj_loader.h>
#include "RBCommon.h"
#include "RBPipelineManager.h"
#include "RBShaderModule.h"
#include <stdexcept>
#include <iostream>
#include "RBPipelineConfig.h"

namespace RottenBamboo {
    class RBGraphicPipelineManager : public RBPipelineManager {

     protected:

        RBPipelineConfig rbPipelineConfig;

        void setupShaders() override;

        void setupPipelineStates() override;

    public:

        void createGraphicsPipelines(const VkGraphicsPipelineCreateInfo &pipelineInfo) override;

        ~RBGraphicPipelineManager() override;

        void createGraphicsPipeline() override;

        void InitializeGraphicPipeline() override;

        RBGraphicPipelineManager(RBDevice &device, RBSwapChain &swapChain, RBDescriptors &descriptors, const RBPipelineConfig &config);

        void fillGraphicsPipelineCreateInfo(uint32_t stageCount,
                                            const VkPipelineShaderStageCreateInfo* pStages,
                                            const VkPipelineVertexInputStateCreateInfo* pVertexInputState,
                                            const VkPipelineInputAssemblyStateCreateInfo* pInputAssemblyState,
                                            const VkPipelineTessellationStateCreateInfo* pTessellationState,
                                            const VkPipelineViewportStateCreateInfo* pViewportState,
                                            const VkPipelineRasterizationStateCreateInfo* pRasterizationState,
                                            const VkPipelineMultisampleStateCreateInfo* pMultisampleState,
                                            const VkPipelineDepthStencilStateCreateInfo* pDepthStencilState,
                                            const VkPipelineColorBlendStateCreateInfo* pColorBlendState,
                                            const VkPipelineDynamicStateCreateInfo* pDynamicState,
                                            VkPipelineLayout layout,
                                            VkRenderPass renderPass,
                                            uint32_t subpass,
                                            VkPipeline basePipelineHandle,
                                            int32_t basePipelineIndex
        ) override;

        virtual void fillShaderModule(const std::string& shaderName, VkShaderStageFlagBits stage, const char* pName, RBShaderModule &shaderModule);

    private:

        RBShaderModule vertShaderModule;

        RBShaderModule fragShaderModule;
    };
}

//#endif //VULKANFROMSCRATCH_RBGRAPHICPIPELINEMANAGER_H
