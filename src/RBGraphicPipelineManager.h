//
// Created by rottenbamboo on 2023/5/22.
//

#pragma once
#ifndef VULKANFROMSCRATCH_RBGRAPHICPIPELINEMANAGER_H
#define VULKANFROMSCRATCH_RBGRAPHICPIPELINEMANAGER_H

#include <fstream>
#include <tiny_obj_loader.h>
#include "RBCommon.h"
#include "RBSwapChain.h"
#include <stdexcept>
#include <iostream>

namespace RottenBamboo {
    class RBGraphicPipelineManager {
    public:

        VkPipelineLayout pipelineLayout;

        VkPipeline graphicsPipeline;

        RBGraphicPipelineManager(RBSwapChain &swapChain, RBDescriptors &descriptors);

        ~RBGraphicPipelineManager();

        void createGraphicsPipeline();

        void InitializeGraphicPipeline();

        void fillShaderModule(const std::string& shaderName, VkShaderStageFlagBits stage, const char* pName);

        void fillVertexInputStateCreateInfo();

        void fillInputAssemblyStateCreateInfo(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable);

        void fillViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t minDepth, uint32_t maxDepth);

        void fillScissor(VkOffset2D offset, VkExtent2D extent);

        void fillViewportStateCreateInfo();

        void fillRasterizerStateCreateInfo(VkBool32 depthClampEnable,
                                                                     VkBool32 rasterizerDiscardEnable,
                                                                     VkPolygonMode polygonMode,
                                                                     VkCullModeFlags cullMode,
                                                                     VkFrontFace frontFace,
                                                                     VkBool32 depthBiasEnable,
                                                                     float depthBiasConstantFactor,
                                                                     float depthBiasClamp,
                                                                     float depthBirasSlopeFactor,
                                                                     float lineWidth);

        void fillMultipleSampleStateCreateInfo(VkPipelineMultisampleStateCreateFlags flags,
                                               VkSampleCountFlagBits rasterizationSamples,
                                               VkBool32 sampleShadingEnable,
                                               float minSampleShading,
                                               const VkSampleMask* pSampleMask,
                                               VkBool32 alphaToCoverageEnable,
                                               VkBool32 alphaToOneEnable);

        void fillDepthStencilStateCreateInfo(VkPipelineDepthStencilStateCreateFlags flags,
                                             VkBool32 depthTestEnable,
                                             VkBool32 depthWriteEnable,
                                             VkCompareOp depthCompareOp,
                                             VkBool32 depthBoundsTestEnable,
                                             VkBool32 stencilTestEnable,
                                             VkStencilOpState front,
                                             VkStencilOpState back,
                                             float minDepthBounds,
                                             float maxDepthBounds);

        void fillColorBlendAttachmentState(VkBool32 blendEnable,
                                           VkBlendFactor srcColorBlendFactor,
                                           VkBlendFactor dstColorBlendFactor,
                                           VkBlendOp colorBlendOp,
                                           VkBlendFactor srcAlphaBlendFactor,
                                           VkBlendFactor dstAlphaBlendFactor,
                                           VkBlendOp alphaBlendOp,
                                           VkColorComponentFlags colorWriteMask);

        void fillPipelineColorBlendStateCreateInfo(VkBool32 logicOpEnable,
                                                   VkLogicOp logicOp,
                                                   uint32_t attachmentCount,
                                                   const VkPipelineColorBlendAttachmentState* pAttachments,
                                                   float blendConstants[4]);

        void fillDynamicStateCrateInfo();

        void fillPipelineLayoutInfo();

        void createPipelineLayout();

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
        );

        void createGraphicsPipelines();

    private:

        RBSwapChain &rbSwapChain;
        RBDescriptors &rbDescriptors;

        static std::vector<char> readFile(const std::string &filename);

        VkShaderModule createShaderModule(const std::vector<char> &code);

        VkShaderModule vertShaderModule;

        VkShaderModule fragShaderModule;

        std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfos{};

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};

        VkVertexInputBindingDescription bindingDescription;

        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions;

        VkViewport viewport{};

        VkRect2D scissor{};

        VkPipelineViewportStateCreateInfo viewportState{};

        VkPipelineRasterizationStateCreateInfo rasterizer{};

        VkPipelineMultisampleStateCreateInfo multisampling{};

        VkPipelineDepthStencilStateCreateInfo depthStencil{};

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};

        VkPipelineColorBlendStateCreateInfo colorBlending{};

        std::vector<VkDynamicState> dynamicStates;

        VkPipelineDynamicStateCreateInfo dynamicState{};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

        VkGraphicsPipelineCreateInfo pipelineInfo{};
    };
}

#endif //VULKANFROMSCRATCH_RBGRAPHICPIPELINEMANAGER_H
