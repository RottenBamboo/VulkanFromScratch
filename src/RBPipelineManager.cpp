#include "RBPipelineManager.h"
#include <stdexcept>
#include <iostream>

namespace RottenBamboo {

    RBPipelineManager::RBPipelineManager(int colorAttachmentCount, bool bResolveAttachment, bool bDephAttament, RBDevice &device, VkImageLayout layout)
        : rbDevice(device), rbColorAttachmentCount(colorAttachmentCount), isResolveAttachment(bResolveAttachment), isDepthAttachment(bDephAttament), imageLayout(layout)
        {
            pureColorAttachmentCount = rbColorAttachmentCount - (isDepthAttachment ? 1 : 0);
            depthAttachmentCount = isDepthAttachment ? 1 : 0;
            ColorAttachKind = 1 + (isResolveAttachment ? 1 : 0);
        }

    RBPipelineManager::~RBPipelineManager() 
    {
        vkDestroyPipeline(rbDevice.device, graphicsPipeline, nullptr);
        std::cout << "RBPipelineManager::~RBPipelineManager()" << std::endl;
    }

    void RBPipelineManager::createGraphicsPipelines(const VkGraphicsPipelineCreateInfo &pipelineInfo) 
    {
        if (vkCreateGraphicsPipelines(rbDevice.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }
        std::cout << "RBPipelineManager::createGraphicsPipelines()" << std::endl;
    }

    void RBPipelineManager::fillVertexInputStateCreateInfo()
    {
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.pNext = nullptr;

        bindingDescription = Vertex::getBindingDescription();
        attributeDescriptions = Vertex::getAttributeDescriptions();

        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());

        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        std::cout << "RBPipelineManager::~fillVertexInputInfo()" << std::endl;
    }

    void RBPipelineManager::fillInputAssemblyStateCreateInfo(VkPrimitiveTopology typology, VkBool32 primitiveRestartEnable)
    {
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.pNext = nullptr;
        inputAssembly.topology = typology;
        inputAssembly.primitiveRestartEnable = primitiveRestartEnable;
        std::cout << "RBPipelineManager::~fillInputAssembly()" << std::endl;
    }

    void RBPipelineManager::fillViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t minDepth, uint32_t maxDepth)
    {
        viewport.x = x;
        viewport.y = y;
        viewport.width = width;
        viewport.height = height;
        viewport.minDepth = minDepth;
        viewport.maxDepth = maxDepth;
        std::cout << "RBPipelineManager::~fillViewport()" << std::endl;
    }

    void RBPipelineManager::fillScissor(VkOffset2D offset, VkExtent2D extent)
    {
        scissor.offset = offset;
        scissor.extent = extent;
        std::cout << "RBPipelineManager::fillScissor()" << std::endl;
    }

    void RBPipelineManager::fillViewportStateCreateInfo()
    {
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.pNext = nullptr;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;
        std::cout << "RBPipelineManager::fillViewportStateCreateInfo()" << std::endl;
    }

    void RBPipelineManager::fillRasterizerStateCreateInfo(VkBool32 depthClampEnable,
                                                                 VkBool32 rasterizerDiscardEnable,
                                                                 VkPolygonMode polygonMode,
                                                                 VkCullModeFlags cullMode,
                                                                 VkFrontFace frontFace,
                                                                 VkBool32 depthBiasEnable,
                                                                 float depthBiasConstantFactor,
                                                                 float depthBiasClamp,
                                                                 float depthBirasSlopeFactor,
                                                                 float lineWidth)
    {
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = depthClampEnable;
        rasterizer.pNext = nullptr;
        rasterizer.rasterizerDiscardEnable = rasterizerDiscardEnable;
        rasterizer.polygonMode = polygonMode;
        rasterizer.lineWidth = lineWidth;
        rasterizer.cullMode = cullMode;
        //rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.frontFace = frontFace;
        rasterizer.depthBiasEnable = depthBiasEnable;
        rasterizer.depthBiasConstantFactor = depthBiasConstantFactor;
        rasterizer.depthBiasClamp = depthBiasClamp;
        std::cout << "RBPipelineManager::fillRasterizerStateCreateInfo()" << std::endl;
    }

    void RBPipelineManager::fillMultipleSampleStateCreateInfo(VkPipelineMultisampleStateCreateFlags flags,
                                                                     VkSampleCountFlagBits rasterizationSamples,
                                                                     VkBool32 sampleShadingEnable,
                                                                     float minSampleShading,
                                                                     const VkSampleMask* pSampleMask,
                                                                     VkBool32 alphaToCoverageEnable,
                                                                     VkBool32 alphaToOneEnable)
    {
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.pNext = nullptr;
        multisampling.flags = flags;
        multisampling.rasterizationSamples = rasterizationSamples;
        multisampling.sampleShadingEnable = sampleShadingEnable;
        multisampling.minSampleShading = minSampleShading;
        multisampling.pSampleMask = pSampleMask;
        multisampling.alphaToCoverageEnable = alphaToCoverageEnable;
        multisampling.alphaToOneEnable = alphaToOneEnable;
        std::cout << "RBPipelineManager::fillMultipleSampleStateCreateInfo()" << std::endl;
    }

    void RBPipelineManager::fillColorBlendAttachmentState(VkBool32 blendEnable,
                                                                 VkBlendFactor srcColorBlendFactor,
                                                                 VkBlendFactor dstColorBlendFactor,
                                                                 VkBlendOp colorBlendOp,
                                                                 VkBlendFactor srcAlphaBlendFactor,
                                                                 VkBlendFactor dstAlphaBlendFactor,
                                                                 VkBlendOp alphaBlendOp,
                                                                 VkColorComponentFlags colorWriteMask)
    {
        colorBlendAttachments.reserve(rbColorAttachmentCount);
        colorBlendAttachments.clear();
        for(int i = 0; i < rbColorAttachmentCount; i++)
        {
            VkPipelineColorBlendAttachmentState colorBlendAttachment;
            colorBlendAttachment.blendEnable = blendEnable;
            colorBlendAttachment.srcColorBlendFactor = srcColorBlendFactor;
            colorBlendAttachment.dstColorBlendFactor = dstColorBlendFactor;
            colorBlendAttachment.colorBlendOp = colorBlendOp;
            colorBlendAttachment.srcAlphaBlendFactor = srcAlphaBlendFactor;
            colorBlendAttachment.dstAlphaBlendFactor = dstAlphaBlendFactor;
            colorBlendAttachment.alphaBlendOp = alphaBlendOp;
            colorBlendAttachment.colorWriteMask = colorWriteMask;
            colorBlendAttachments.push_back(colorBlendAttachment);
        }
        std::cout << "RBPipelineManager::fillColorBlendAttachmentState()" << std::endl;
    }

    void RBPipelineManager::fillPipelineColorBlendStateCreateInfo(VkBool32 logicOpEnable,
                                                                         VkLogicOp logicOp,
                                                                         uint32_t attachmentCount,
                                                                         const VkPipelineColorBlendAttachmentState* pAttachments,
                                                                         float blendConstants[4])
    {
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.pNext = nullptr;
        colorBlending.flags = 0;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_AND;
        colorBlending.attachmentCount = attachmentCount;
        colorBlending.pAttachments = pAttachments;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;
        std::cout << "RBPipelineManager::fillPipelineColorBlendStateCreateInfo()" << std::endl;
    }

    void RBPipelineManager::fillDynamicStateCrateInfo()
    {
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();
        std::cout << "RBPipelineManager::fillDynamicStateCrateInfo()" << std::endl;

    }
    
    VkFormat RBPipelineManager::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
    {
        for(VkFormat format : candidates)
        {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(rbDevice.physicalDevice, format, &props);

            if(tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
            {
                return format;
            }
            else if(tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
            {
                return format;
            }
        }

        throw std::runtime_error("failed to find supported format!");

    }
    
    VkFormat RBPipelineManager::findDepthFormat()
    {
        return findSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }
      
    void RBPipelineManager::fillColorResolveAttachment(VkFormat format,
                                    VkSampleCountFlagBits samples,
                                    VkAttachmentLoadOp loadOp,
                                    VkAttachmentStoreOp storeOp,
                                    VkAttachmentLoadOp stencilLoadOp,
                                    VkAttachmentStoreOp stencilStoreOp,
                                    VkImageLayout initialLayout,
                                    VkImageLayout finalLayout)
        {
            colorResolveAttachmentDescription.format = format;
            colorResolveAttachmentDescription.samples = samples;
            colorResolveAttachmentDescription.loadOp = loadOp;
            colorResolveAttachmentDescription.storeOp = storeOp;
            colorResolveAttachmentDescription.stencilLoadOp = stencilLoadOp;
            colorResolveAttachmentDescription.stencilStoreOp = stencilStoreOp;
            colorResolveAttachmentDescription.initialLayout = initialLayout;
            colorResolveAttachmentDescription.finalLayout = finalLayout;
            std::cout << "RBPipelineManager::fillColorResolveAttachment()" << std::endl;
        }

    void RBPipelineManager::fillColorAttachment(VkFormat format,
                             VkSampleCountFlagBits samples,
                             VkAttachmentLoadOp loadOp,
                             VkAttachmentStoreOp storeOp,
                             VkAttachmentLoadOp stencilLoadOp,
                             VkAttachmentStoreOp stencilStoreOp,
                             VkImageLayout initialLayout,
                             VkImageLayout finalLayout)
        {
            colorAttachmentDescription.format = format;
            colorAttachmentDescription.samples = samples;
            colorAttachmentDescription.loadOp = loadOp;
            colorAttachmentDescription.storeOp = storeOp;
            colorAttachmentDescription.stencilLoadOp = stencilLoadOp;
            colorAttachmentDescription.stencilStoreOp = stencilStoreOp;
            colorAttachmentDescription.initialLayout = initialLayout;
            colorAttachmentDescription.finalLayout = finalLayout;
            std::cout << "RBPipelineManager::fillColorAttachment()" << std::endl;
        }

    void RBPipelineManager::fillDepthAttachment(VkFormat format,
                             VkSampleCountFlagBits samples,
                             VkAttachmentLoadOp loadOp,
                             VkAttachmentStoreOp storeOp,
                             VkAttachmentLoadOp stencilLoadOp,
                             VkAttachmentStoreOp stencilStoreOp,
                             VkImageLayout initialLayout,
                             VkImageLayout finalLayout)
        {
            depthAttachmentDescription.format = format;
            depthAttachmentDescription.samples = samples;
            depthAttachmentDescription.loadOp = loadOp;
            depthAttachmentDescription.storeOp = storeOp;
            depthAttachmentDescription.stencilLoadOp = stencilLoadOp;
            depthAttachmentDescription.stencilStoreOp = stencilStoreOp;
            depthAttachmentDescription.initialLayout = initialLayout;
            depthAttachmentDescription.finalLayout = finalLayout;
            std::cout << "RBPipelineManager::fillColorAttachment()" << std::endl;
        }

    void RBPipelineManager::addColorAttachment(VkAttachmentDescription attachment)
    {
        attachmentDescriptions.push_back(attachment);
        std::cout << "RBPipelineManager::addColorAttachment()" << std::endl;
    }

    void RBPipelineManager::fillRenderPass(VkImageLayout layout, int attachmentCount = 1)
    {
        attachmentDescriptions.clear();
        attachmentDescriptions.reserve(depthAttachmentCount + pureColorAttachmentCount * ColorAttachKind);


        for(int i = 0; i < pureColorAttachmentCount; i++)
        {
            addColorAttachment(colorAttachmentDescription);
            
        }
        
        for(int i = 0; i < pureColorAttachmentCount; i++)
        {
            if(isResolveAttachment)
            {
                addColorAttachment(colorResolveAttachmentDescription);
            }
        }
        std::cout << "isResolveAttachment = " << isResolveAttachment << std::endl;

        std::cout << "rbColorAttachmentCount = " << pureColorAttachmentCount << std::endl;
        //addColorAttachment(surfaceFormat.format, msaaSamples, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

        if(isDepthAttachment)
        {
            addColorAttachment(depthAttachmentDescription);
        }        
        std::cout << "isDepthAttachment = " << isDepthAttachment << std::endl;

        //VkAttachmentReference colorAttachmentRef{};
        //colorAttachmentRef.attachment = 0;
        //colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        std::vector<VkAttachmentReference> colorAttachmentRefs(pureColorAttachmentCount);
        
        for (int i = 0; i < pureColorAttachmentCount; i++) {
            colorAttachmentRefs[i].attachment = i;
            colorAttachmentRefs[i].layout = layout;
        }

        std::vector<VkAttachmentReference> colorAttachmentResolveRef(pureColorAttachmentCount);

        if(isResolveAttachment)
        {
            for(int i = 0; i < pureColorAttachmentCount; i++)
            {
                colorAttachmentResolveRef[i].attachment = i + pureColorAttachmentCount;
                colorAttachmentResolveRef[i].layout = layout;
            }
        }

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = pureColorAttachmentCount * ColorAttachKind;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = pureColorAttachmentCount;
        subpass.pColorAttachments = colorAttachmentRefs.data();
        subpass.pDepthStencilAttachment = isDepthAttachment ? &depthAttachmentRef : nullptr;
        subpass.pResolveAttachments = isResolveAttachment ? colorAttachmentResolveRef.data() : nullptr;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = 0;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size());
        renderPassInfo.pAttachments = attachmentDescriptions.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if(vkCreateRenderPass(rbDevice.device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
        {
            throw::std::runtime_error("failed to create render pass!");
        }
        std::cout << "RBPipelineManager::fillRenderPass()" << std::endl;
    }

    void RBPipelineManager::fillGraphicsPipelineCreateInfo(uint32_t stageCount,
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
        )
    {
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStageInfos.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pTessellationState = nullptr;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = nullptr;
        pipelineInfo.layout = rbPipelineLayoutManager.pipelineLayout;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.basePipelineIndex = -1;
        std::cout << "RBPipelineManager::fillGraphicsPipelineCreateInfo()" << std::endl;
    
    }
    
    void RBPipelineManager::setupPipelineStates()
    {
        fillVertexInputStateCreateInfo();
        fillInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);
        fillViewport(0, 0, swapChainExtent.width, swapChainExtent.height, 0, 1);
        VkOffset2D offset{0, 0};
        fillScissor(offset, swapChainExtent);
        fillViewportStateCreateInfo();
        setupAttachments();
        fillRenderPass(imageLayout, rbColorAttachmentCount);
        createFrameBuffers();
        fillRasterizerStateCreateInfo(VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f);
        fillMultipleSampleStateCreateInfo(0, msaaSamples, VK_TRUE, 0.2f, nullptr, VK_FALSE, VK_FALSE);
        //fillDepthStencilStateCreateInfo(0, VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS, VK_FALSE, VK_FALSE, {}, {}, 0.0f, 1.0f);
        fillDepthStencilStateCreateInfo();
        fillColorBlendAttachmentState(VK_TRUE, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                      VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
        float blendConstants[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        fillPipelineColorBlendStateCreateInfo(VK_FALSE, VK_LOGIC_OP_AND, pureColorAttachmentCount, colorBlendAttachments.data(), blendConstants);
        
        dynamicStates.clear();
        dynamicStates.reserve(0);
        dynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
        dynamicStates.push_back(VK_DYNAMIC_STATE_LINE_WIDTH);
        fillDynamicStateCrateInfo();
    }
}