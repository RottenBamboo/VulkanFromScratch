//
// Created by rottenbamboo on 2023/5/22.
//

#pragma once

#include <fstream>
#include "RBCommon.h"
#include "RBPipelineManager.h"
#include "RBPipelineConfig.h"
#include "RBShaderModule.h" // Added to define RBShaderModule
#include <stdexcept>
#include <iostream>

namespace RottenBamboo {
    class RBGBufferPass : public RBPipelineManager {

     protected:

        RBPipelineConfig rbPipelineConfig;

        RBDescriptors<1> &rbDescriptors;

        void setupShaders() override;

        void setupPipelineStates() override;

    public:

        void createGraphicsPipelines(const VkGraphicsPipelineCreateInfo &pipelineInfo) override;

        ~RBGBufferPass() override;

        void createGraphicsPipeline() override;

        void InitializeGraphicPipeline() override;

        RBGBufferPass(RBDevice &device, RBDescriptors<1> &descriptors, const RBPipelineConfig &config);

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

