#pragma once

#include "RBCommon.h"
#include "RBDevice.h"
#include "RBSwapChain.h"
#include "RBPipelineLayoutManager.h"
#include <vector>

namespace RottenBamboo {
    class RBPipelineManager {
    protected:

        int rbColorAttachmentCount = 1;

        bool isResolveAttachment = false;

        bool isDepthAttachment = true;

        RBDevice &rbDevice;

        VkRenderPass renderPass;

        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions;

        VkVertexInputBindingDescription bindingDescription;

        VkAttachmentDescription colorAttachmentDescription{}; 

        VkAttachmentDescription colorResolveAttachmentDescription{}; 

        VkAttachmentDescription depthAttachmentDescription{}; 

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
        
        virtual void setupAttachments() = 0;

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
                                                    uint32_t subpass,
                                                    VkPipeline basePipelineHandle,
                                                    int32_t basePipelineIndex);

        virtual VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        virtual VkFormat findDepthFormat();

        virtual void fillRenderPass(int attachmentCount);

        virtual void fillDynamicStateCrateInfo();

        virtual void createGraphicsPipelines(const VkGraphicsPipelineCreateInfo &pipelineInfo) = 0;

        virtual void addColorAttachment(VkAttachmentDescription attachment);

        virtual void fillColorResolveAttachment(VkFormat format,
            VkSampleCountFlagBits samples,
            VkAttachmentLoadOp loadOp,
            VkAttachmentStoreOp storeOp,
            VkAttachmentLoadOp stencilLoadOp,
            VkAttachmentStoreOp stencilStoreOp,
            VkImageLayout initialLayout,
            VkImageLayout finalLayout);

        virtual void fillColorAttachment(VkFormat format,
            VkSampleCountFlagBits samples,
            VkAttachmentLoadOp loadOp,
            VkAttachmentStoreOp storeOp,
            VkAttachmentLoadOp stencilLoadOp,
            VkAttachmentStoreOp stencilStoreOp,
            VkImageLayout initialLayout,
            VkImageLayout finalLayout);

        virtual void fillDepthAttachment(VkFormat format,
            VkSampleCountFlagBits samples,
            VkAttachmentLoadOp loadOp,
            VkAttachmentStoreOp storeOp,
            VkAttachmentLoadOp stencilLoadOp,
            VkAttachmentStoreOp stencilStoreOp,
            VkImageLayout initialLayout,
            VkImageLayout finalLayout);

    public:

        VkPipeline graphicsPipeline;

        RBPipelineLayoutManager rbPipelineLayoutManager{rbDevice};

        RBPipelineManager(int colorAttachmentCount, bool bResolveAttachment, bool bDephAttament, RBDevice &device);

        virtual void createGraphicsPipeline() = 0;

        virtual void InitializeGraphicPipeline() = 0;

        virtual ~RBPipelineManager();

        private:

        std::vector<VkAttachmentDescription> attachmentDescriptions;
    };
}