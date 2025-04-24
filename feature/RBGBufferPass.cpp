//
// Created by rottenbamboo on 2023/5/22.
//

#include "RBPipelineUtils.h"
#include "RBGBufferPass.h"

namespace RottenBamboo {

    void RBGBufferPass::setupShaders()
    {
        fillShaderModule("../shader/gBufferVert.spv", VK_SHADER_STAGE_VERTEX_BIT, "main", vertShaderModule);
        fillShaderModule("../shader/gBufferFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT, "main", fragShaderModule);
    }

    void RBGBufferPass::fillShaderModule(const std::string& shaderName, VkShaderStageFlagBits stage, const char* pName, RBShaderModule &shaderModule)
    {
        auto shaderCode = RBPipelineUtils::readFile(shaderName);
        VkPipelineShaderStageCreateInfo shaderStageInfo{};

        shaderModule.createShaderModule(rbDevice, shaderCode);

        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.pNext = nullptr;
        shaderStageInfo.stage = stage;
        shaderStageInfo.module = shaderModule.get();
        shaderStageInfo.pName = "main";
        shaderStageInfos.push_back(shaderStageInfo);
        std::cout << "RBGBufferPass::fillShaderModule()" << std::endl;

    }

    void RBGBufferPass::setupPipelineStates()
    {
        RBPipelineManager::setupPipelineStates();
        std::cout << "RBGBufferPass::setupPipelineStates()" << std::endl;
    }

    void RBGBufferPass::createGraphicsPipelines(const VkGraphicsPipelineCreateInfo &pipelineInfo)
    {
        RBPipelineManager::createGraphicsPipelines(pipelineInfo);
        std::cout << "RBGBufferPass::createGraphicsPipelines()" << std::endl;
    }

    void RBGBufferPass::fillGraphicsPipelineCreateInfo(uint32_t stageCount,
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
        RBPipelineManager::fillGraphicsPipelineCreateInfo(stageCount, pStages, pVertexInputState, pInputAssemblyState, pTessellationState, pViewportState, pRasterizationState, pMultisampleState, pDepthStencilState, pColorBlendState, pDynamicState, layout, subpass, basePipelineHandle, basePipelineIndex);
        std::cout << "RBGBufferPass::fillGraphicsPipelineCreateInfo()" << std::endl;
    }

    void RBGBufferPass::createGraphicsPipeline()
    {
        setupShaders();

        setupPipelineStates();

        rbPipelineLayoutManager.fillPipelineLayoutInfo(&rbDescriptors.descriptorSetManager.descriptorSetLayoutManager.descriptorSetLayout);
        rbPipelineLayoutManager.createPipelineLayout();

        fillGraphicsPipelineCreateInfo(2, shaderStageInfos.data(), &vertexInputInfo, &inputAssembly, nullptr, &viewportState, &rasterizer, &multisampling, &depthStencil, &colorBlending, nullptr, rbPipelineLayoutManager.pipelineLayout, 0, VK_NULL_HANDLE, -1);

        createGraphicsPipelines(pipelineInfo);

        std::cout << "RBGBufferPass::createGraphicsPipeline()" << std::endl;
    }

    RBGBufferPass::RBGBufferPass(RBDevice &device, RBDescriptors<4> &descriptors, const RBPipelineConfig &config)
        : RBPipelineManager(device), rbPipelineConfig(config),
        vertShaderModule(device), fragShaderModule(device), rbDescriptors(descriptors) 
    {
        std::cout << "RBGBufferPass::RBGBufferPass()" << std::endl;
    }

    void RBGBufferPass::InitializeGraphicPipeline()
    {
        createGraphicsPipeline();
        std::cout << "RBGBufferPass::InitializeGraphicPipeline()" << std::endl;
    }

    RBGBufferPass::~RBGBufferPass()
    {
        vkDestroyDescriptorSetLayout(rbDevice.device, rbDescriptors.descriptorSetManager.descriptorSetLayoutManager.descriptorSetLayout, nullptr);
        std::cout << "RBGBufferPass::~RBGBufferPass()" << std::endl;
    }
}
