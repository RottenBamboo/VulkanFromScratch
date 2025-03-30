//
// Created by rottenbamboo on 2023/5/22.
//

#include "RBGraphicPipelineManager.h"

namespace RottenBamboo {

    VkShaderModule RBGraphicPipelineManager::createShaderModule(const std::vector<char> &code) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(rbSwapChain.refDevice.device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw ::std::runtime_error("failed to create shader module!");
        }
        return shaderModule;
    }

    std::vector<char> RBGraphicPipelineManager::readFile(const std::string &filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }
        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }

    void RBGraphicPipelineManager::fillShaderModule(const std::string& shaderName, VkShaderStageFlagBits stage, const char* pName)
    {
        auto shaderCode = readFile(shaderName);
        VkPipelineShaderStageCreateInfo shaderStageInfo{};

        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.pNext = nullptr;
        shaderStageInfo.stage = stage;
        shaderStageInfo.module = createShaderModule(shaderCode);
        shaderStageInfo.pName = "main";
        shaderStageInfos.push_back(shaderStageInfo);
        std::cout << "RBGraphicPipelineManager::~fillShaderModule()" << std::endl;

    }

    void RBGraphicPipelineManager::fillVertexInputStateCreateInfo()
    {
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.pNext = nullptr;

        bindingDescription = Vertex::getBindingDescription();
        attributeDescriptions = Vertex::getAttributeDescriptions();

        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());

        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        std::cout << "RBGraphicPipelineManager::~fillVertexInputInfo()" << std::endl;
    }

    void RBGraphicPipelineManager::fillInputAssemblyStateCreateInfo(VkPrimitiveTopology typology, VkBool32 primitiveRestartEnable)
    {
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.pNext = nullptr;
        inputAssembly.topology = typology;
        inputAssembly.primitiveRestartEnable = primitiveRestartEnable;
        std::cout << "RBGraphicPipelineManager::~fillInputAssembly()" << std::endl;
    }

    void RBGraphicPipelineManager::fillViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t minDepth, uint32_t maxDepth)
    {
        viewport.x = x;
        viewport.y = y;
        viewport.width = width;
        viewport.height = height;
        viewport.minDepth = minDepth;
        viewport.maxDepth = maxDepth;
        std::cout << "RBGraphicPipelineManager::~fillViewport()" << std::endl;
    }

    void RBGraphicPipelineManager::fillScissor(VkOffset2D offset, VkExtent2D extent)
    {
        scissor.offset = offset;
        scissor.extent = extent;
    }

    void RBGraphicPipelineManager::fillViewportStateCreateInfo()
    {
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.pNext = nullptr;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;
    }

    void RBGraphicPipelineManager::fillRasterizerStateCreateInfo(VkBool32 depthClampEnable,
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
        rasterizer.depthBiasSlopeFactor = depthBirasSlopeFactor;
    }

    void RBGraphicPipelineManager::fillMultipleSampleStateCreateInfo(VkPipelineMultisampleStateCreateFlags flags,
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
    }

    void RBGraphicPipelineManager::fillDepthStencilStateCreateInfo(VkPipelineDepthStencilStateCreateFlags flags,
                                                                   VkBool32 depthTestEnable,
                                                                   VkBool32 depthWriteEnable,
                                                                   VkCompareOp depthCompareOp,
                                                                   VkBool32 depthBoundsTestEnable,
                                                                   VkBool32 stencilTestEnable,
                                                                   VkStencilOpState front,
                                                                   VkStencilOpState back,
                                                                   float minDepthBounds,
                                                                   float maxDepthBounds)
    {
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.flags = flags;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = front;
        depthStencil.back = back;
        depthStencil.minDepthBounds = 0.0f;
        depthStencil.maxDepthBounds = 1.0f;
    }

    void RBGraphicPipelineManager::fillColorBlendAttachmentState(VkBool32 blendEnable,
                                                                 VkBlendFactor srcColorBlendFactor,
                                                                 VkBlendFactor dstColorBlendFactor,
                                                                 VkBlendOp colorBlendOp,
                                                                 VkBlendFactor srcAlphaBlendFactor,
                                                                 VkBlendFactor dstAlphaBlendFactor,
                                                                 VkBlendOp alphaBlendOp,
                                                                 VkColorComponentFlags colorWriteMask)
    {
        colorBlendAttachment.blendEnable = blendEnable;
        colorBlendAttachment.srcColorBlendFactor = srcColorBlendFactor;
        colorBlendAttachment.dstColorBlendFactor = dstColorBlendFactor;
        colorBlendAttachment.colorBlendOp = colorBlendOp;
        colorBlendAttachment.srcAlphaBlendFactor = srcAlphaBlendFactor;
        colorBlendAttachment.dstAlphaBlendFactor = dstAlphaBlendFactor;
        colorBlendAttachment.alphaBlendOp = alphaBlendOp;
        colorBlendAttachment.colorWriteMask = colorWriteMask;
    }

    void RBGraphicPipelineManager::fillPipelineColorBlendStateCreateInfo(VkBool32 logicOpEnable,
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
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = pAttachments;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;
    }

    void RBGraphicPipelineManager::fillDynamicStateCrateInfo()
    {
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

    }

    void RBGraphicPipelineManager::fillPipelineLayoutInfo()
    {
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &rbDescriptors.descriptorSetManager.descriptorSetLayoutManager.descriptorSetLayout;
    }

    void RBGraphicPipelineManager::createPipelineLayout()
    {
        if (vkCreatePipelineLayout(rbSwapChain.refDevice.device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw ::std::runtime_error("failed to create pipeline layout");
        }
    }

    void RBGraphicPipelineManager::fillGraphicsPipelineCreateInfo(uint32_t stageCount,
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
        pipelineInfo.layout = pipelineLayout;
        pipelineInfo.renderPass = rbSwapChain.renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.basePipelineIndex = -1;

    }

    void RBGraphicPipelineManager::createGraphicsPipelines()
    {
        if (vkCreateGraphicsPipelines(rbSwapChain.refDevice.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
            throw ::std::runtime_error("failed to create graphics pipeline");
        }
    }

    void RBGraphicPipelineManager::createGraphicsPipeline() {

        fillShaderModule("../shader/vert.spv", VK_SHADER_STAGE_VERTEX_BIT, "main");
        fillShaderModule("../shader/frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT, "main");

        fillVertexInputStateCreateInfo();

        fillInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);

        fillViewport(0, 0, swapChainExtent.width, swapChainExtent.height, 0, 1);

        VkOffset2D offset{0, 0};
        fillScissor(offset, swapChainExtent);

        fillViewportStateCreateInfo();

        fillRasterizerStateCreateInfo(VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f);

        fillMultipleSampleStateCreateInfo(0, msaaSamples, VK_TRUE, 0.2f, nullptr, VK_FALSE, VK_FALSE);

        fillDepthStencilStateCreateInfo(0, VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS, VK_FALSE, VK_FALSE, {}, {}, 0.0f, 1.0f);

        fillColorBlendAttachmentState(VK_FALSE, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                      VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);

        float blendConstants[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        fillPipelineColorBlendStateCreateInfo(VK_FALSE, VK_LOGIC_OP_AND, 1, &colorBlendAttachment, blendConstants);

        dynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
        dynamicStates.push_back(VK_DYNAMIC_STATE_LINE_WIDTH);
        fillDynamicStateCrateInfo();

        fillPipelineLayoutInfo();
        createPipelineLayout();
        fillGraphicsPipelineCreateInfo(2, shaderStageInfos.data(), &vertexInputInfo, &inputAssembly, nullptr, &viewportState, &rasterizer, &multisampling, &depthStencil, &colorBlending, nullptr, pipelineLayout, rbSwapChain.renderPass, 0, VK_NULL_HANDLE, -1);

        createGraphicsPipelines();

        vkDestroyShaderModule(rbSwapChain.refDevice.device, vertShaderModule, nullptr);
        vkDestroyShaderModule(rbSwapChain.refDevice.device, fragShaderModule, nullptr);
    }

    RBGraphicPipelineManager::RBGraphicPipelineManager(RBSwapChain &swapChain, RBDescriptors &descriptors) : rbSwapChain(swapChain), rbDescriptors(descriptors) {

    }

    void RBGraphicPipelineManager::InitializeGraphicPipeline()
    {
        createGraphicsPipeline();
    }

    RBGraphicPipelineManager::~RBGraphicPipelineManager() {
        vkDestroyPipeline(rbSwapChain.refDevice.device, graphicsPipeline, nullptr);

        vkDestroyPipelineLayout(rbSwapChain.refDevice.device, pipelineLayout, nullptr);

        vkDestroyDescriptorSetLayout(rbSwapChain.refDevice.device, rbDescriptors.descriptorSetManager.descriptorSetLayoutManager.descriptorSetLayout, nullptr);
    }
}
