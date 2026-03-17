//
// Created by rottenbamboo on 2023/5/22.
//

#pragma once

#include <fstream>
#include "RBCommon.h"
#include "RBPipelineManager.h"
#include "RBPipelineConfig.h"
#include "RBShaderModule.h" // Added to define RBShaderModule
#include "RBResourceManager.h"
#include <stdexcept>
#include <iostream>

namespace RottenBamboo {
    class RBGBufferPass : public RBPipelineManager {

     protected:

        RBPipelineConfig rbPipelineConfig;

        //input image
        RBDescriptors &rbDescriptors;

        void setupShaders() override;

        void setupPipelineStates() override;

        void setupAttachments() override;

        void setResourceCount() override;

        void createFrameBuffers() override;

    public:
    
        void clearFrameBuffers();

        VkFramebuffer gBufferFrameBuffers;
    
        //output image
        RBDescriptors &rbColorAttachmentDescriptors;

        void createGraphicsPipelines(const VkGraphicsPipelineCreateInfo &pipelineInfo) override;

        ~RBGBufferPass() override;

        void createGraphicsPipeline() override;

        void InitializeGraphicPipeline() override;

        RBGBufferPass(int colorAttachmentCount, bool bResolveAttachment, bool bDephAttament, RBDevice &device, RBDescriptors &descriptors, RBDescriptors &descriptorColorAttachment, const RBPipelineConfig &config, VkImageLayout layout);

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

        virtual void Execute(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo renderPassInfo, RBDescriptors& descriptorsGBuffer, ResourceManager& resourceManager);
    private:

        RBShaderModule vertShaderModule;

        RBShaderModule fragShaderModule;
    };
}

