#pragma once

#include "RBCommon.h"
#include "RBDevice.h"
#include "RBSwapChain.h"
#include "RBDescriptors.h"
#include "RBPipelineLayoutManager.h"
#include <vector>

namespace RottenBamboo {
    class RBPipelineManager {
    protected:
        RBDevice &rbDevice;

        RBSwapChain &rbSwapChain;

        RBDescriptors &rbDescriptors;

        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions;

        VkVertexInputBindingDescription bindingDescription;

        VkViewport viewport{};

        VkRect2D scissor{};

        std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfos{};

        VkGraphicsPipelineCreateInfo pipelineInfo{};

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};

        std::vector<VkDynamicState> dynamicStates;

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};

        VkPipelineViewportStateCreateInfo viewportState{};

        VkPipelineRasterizationStateCreateInfo rasterizer{};

        VkPipelineMultisampleStateCreateInfo multisampling{};

        VkPipelineDepthStencilStateCreateInfo depthStencil{};

        VkPipelineColorBlendStateCreateInfo colorBlending{};

        VkPipelineDynamicStateCreateInfo dynamicState{};

        // General Pipeline Creation Process
        virtual void setupShaders() = 0;

        virtual void setupPipelineStates() = 0;

        // Fill Pipeline States
        virtual void fillVertexInputStateCreateInfo();

        virtual void fillInputAssemblyStateCreateInfo(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable);
        
        virtual void fillViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t minDepth, uint32_t maxDepth);

        virtual void fillScissor(VkOffset2D offset, VkExtent2D extent);

        virtual void fillViewportStateCreateInfo();

        virtual void fillRasterizerStateCreateInfo(VkBool32 depthClampEnable,
                                                   VkBool32 rasterizerDiscardEnable,
                                                   VkPolygonMode polygonMode,
                                                   VkCullModeFlags cullMode,
                                                   VkFrontFace frontFace,
                                                   VkBool32 depthBiasEnable,
                                                   float depthBiasConstantFactor,
                                                   float depthBiasClamp,
                                                   float depthBirasSlopeFactor,
                                                   float lineWidth);

        virtual void fillMultipleSampleStateCreateInfo(VkPipelineMultisampleStateCreateFlags flags,
                                                       VkSampleCountFlagBits rasterizationSamples,
                                                       VkBool32 sampleShadingEnable,
                                                       float minSampleShading,
                                                       const VkSampleMask* pSampleMask,
                                                       VkBool32 alphaToCoverageEnable,
                                                       VkBool32 alphaToOneEnable);

        virtual void fillDepthStencilStateCreateInfo(VkPipelineDepthStencilStateCreateFlags flags,
                                                     VkBool32 depthTestEnable,
                                                     VkBool32 depthWriteEnable,
                                                     VkCompareOp depthCompareOp,
                                                     VkBool32 depthBoundsTestEnable,
                                                     VkBool32 stencilTestEnable,
                                                     VkStencilOpState front,
                                                     VkStencilOpState back,
                                                     float minDepthBounds,
                                                     float maxDepthBounds);

        virtual void fillColorBlendAttachmentState(VkBool32 blendEnable,
                                                   VkBlendFactor srcColorBlendFactor,
                                                   VkBlendFactor dstColorBlendFactor,
                                                   VkBlendOp colorBlendOp,
                                                   VkBlendFactor srcAlphaBlendFactor,
                                                   VkBlendFactor dstAlphaBlendFactor,
                                                   VkBlendOp alphaBlendOp,
                                                   VkColorComponentFlags colorWriteMask);

        virtual void fillPipelineColorBlendStateCreateInfo(VkBool32 logicOpEnable,
                                                           VkLogicOp logicOp,
                                                           uint32_t attachmentCount,
                                                           const VkPipelineColorBlendAttachmentState* pAttachments,
                                                           float blendConstants[4]);

        virtual void fillGraphicsPipelineCreateInfo(uint32_t stageCount,
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
                                                    int32_t basePipelineIndex);

        virtual void fillDynamicStateCrateInfo();

        virtual void createGraphicsPipelines(const VkGraphicsPipelineCreateInfo &pipelineInfo) = 0;

    public:


        VkPipeline graphicsPipeline;

        RBPipelineLayoutManager rbPipelineLayoutManager{rbDevice};

        RBPipelineManager(RBDevice &device, RBSwapChain &swapChain, RBDescriptors &descriptors);

        virtual void createGraphicsPipeline() = 0;

        virtual void InitializeGraphicPipeline() = 0;

        virtual ~RBPipelineManager();
    };
}