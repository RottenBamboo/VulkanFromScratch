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

        //input image
        RBDescriptors<TEXTURE_PATHS_MECH_COUNT, 1> &rbDescriptors;

        //output image
        RBDescriptors<TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT, 1> &rbColorAttachmentDescriptors;

        void setupShaders() override;

        void setupPipelineStates() override;

        void setupAttachments() override;

        void createFrameBuffers() override;

    public:

        void createGraphicsPipelines(const VkGraphicsPipelineCreateInfo &pipelineInfo) override;

        ~RBGBufferPass() override;

        void createGraphicsPipeline() override;

        void InitializeGraphicPipeline() override;

        RBGBufferPass(int colorAttachmentCount, bool bResolveAttachment, bool bDephAttament, RBDevice &device, RBDescriptors<TEXTURE_PATHS_MECH_COUNT, 1> &descriptors, RBDescriptors<TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT, 1> &descriptorColorAttachment, const RBPipelineConfig &config);

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

        // void fillColorBlendAttachmentState(VkBool32 blendEnable,
        //                                     VkBlendFactor srcColorBlendFactor,
        //                                     VkBlendFactor dstColorBlendFactor,
        //                                     VkBlendOp colorBlendOp,
        //                                     VkBlendFactor srcAlphaBlendFactor,
        //                                     VkBlendFactor dstAlphaBlendFactor,
        //                                     VkBlendOp alphaBlendOp,
        //                                     VkColorComponentFlags colorWriteMask
        // ) override;

        virtual void fillShaderModule(const std::string& shaderName, VkShaderStageFlagBits stage, const char* pName, RBShaderModule &shaderModule);

    private:

        RBShaderModule vertShaderModule;

        RBShaderModule fragShaderModule;

        std::vector<VkFramebuffer> gBufferFrameBuffers;
    };
}

