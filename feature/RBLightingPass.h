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
#include "../editors/RBGUI.h"
#include <stdexcept>
#include <iostream>

namespace RottenBamboo {
    class RBLightingPass : public RBPipelineManager {

     protected:

        RBPipelineConfig rbPipelineConfig;

        void setupShaders() override;

        void setupPipelineStates() override;

        void setupAttachments() override;

        void setResourceCount() override;

        void createFrameBuffers() override;

    public:

        RBDescriptors &rbDescriptors;

        void createGraphicsPipelines(const VkGraphicsPipelineCreateInfo &pipelineInfo) override;

        ~RBLightingPass() override;

        void createGraphicsPipeline() override;

        void InitializeGraphicPipeline() override;

        RBLightingPass(int colorAttachmentCount, bool bResolveAttachment, bool bDephAttament, RBDevice &device, RBDescriptors &descriptors, const RBPipelineConfig &config, VkImageLayout layout);

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

        virtual void Execute(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo renderPassInfo, RBDescriptors& descriptors, RBGUI& gui, UniformBufferShaderVariables& uniformMatirx);

    private:

        RBShaderModule vertShaderModule;

        RBShaderModule fragShaderModule;
    };
}
