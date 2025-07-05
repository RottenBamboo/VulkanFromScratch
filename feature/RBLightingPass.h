//
// Created by rottenbamboo on 2023/5/22.
//

#pragma once

#include <fstream>
#include "RBCommon.h"
#include "RBPipelineManager.h"
#include "RBPipelineConfig.h"
#include "RBShaderModule.h" // Added to define RBShaderModule
#include "RBMesh.h"
#include "RBGUI.h"
#include <stdexcept>
#include <iostream>

namespace RottenBamboo {
    class RBLightingPass : public RBPipelineManager {

     protected:

        RBPipelineConfig rbPipelineConfig;

        void setupShaders() override;

        void setupPipelineStates() override;

        void setupAttachments() override;

        void createFrameBuffers() override;

    public:

        RBDescriptors<TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT, 1> &rbDescriptors;

        void createGraphicsPipelines(const VkGraphicsPipelineCreateInfo &pipelineInfo) override;

        ~RBLightingPass() override;

        void createGraphicsPipeline() override;

        void InitializeGraphicPipeline() override;

        RBLightingPass(int colorAttachmentCount, bool bResolveAttachment, bool bDephAttament, RBDevice &device, RBDescriptors<TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT, 1> &descriptors, const RBPipelineConfig &config, VkImageLayout layout);

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

        void fillDepthStencilStateCreateInfo() override;

        virtual void fillShaderModule(const std::string& shaderName, VkShaderStageFlagBits stage, const char* pName, RBShaderModule &shaderModule);

        virtual void recordCommandBuffer(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo renderPassInfo, RBDescriptors<TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT, 1>& descriptors, RBMesh &mesh, RBGUI& gui, UniformBufferShaderVariables& uniformMatirx);

    private:

        RBShaderModule vertShaderModule;

        RBShaderModule fragShaderModule;
    };
}
