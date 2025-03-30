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

        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        //depthStencil.minDepthBounds = 0.0f;
        //depthStencil.maxDepthBounds = 1.0f;
        depthStencil.stencilTestEnable = VK_FALSE;
        //depthStencil.front = {};
        //depthStencil.back = {};

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        std::vector<VkDynamicState> dynamicStates = {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_LINE_WIDTH
        };

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &rbDescriptors.descriptorSetManager.descriptorSetLayoutManager.descriptorSetLayout;

        if (vkCreatePipelineLayout(rbSwapChain.refDevice.device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw ::std::runtime_error("failed to create pipeline layout");
        }

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStageInfos.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
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

        if (vkCreateGraphicsPipelines(rbSwapChain.refDevice.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
            throw ::std::runtime_error("failed to create graphics pipeline");
        }

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
