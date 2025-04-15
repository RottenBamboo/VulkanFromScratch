//
// Created by rottenbamboo on 2023/5/22.
//

#include "RBPipelineUtils.h"
#include "RBLightingPass.h"

namespace RottenBamboo {

    void RBLightingPass::setupShaders()
    {
        fillShaderModule("../shader/lightingVert.spv", VK_SHADER_STAGE_VERTEX_BIT, "main", vertShaderModule);
        fillShaderModule("../shader/lightingFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT, "main", fragShaderModule);
    }

    void RBLightingPass::fillShaderModule(const std::string& shaderName, VkShaderStageFlagBits stage, const char* pName, RBShaderModule &shaderModule)
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
        std::cout << "RBLightingPass::~fillShaderModule()" << std::endl;

    }

    void RBLightingPass::setupPipelineStates()
    {
        RBPipelineManager::setupPipelineStates();
        std::cout << "RBLightingPass::setupPipelineStates()" << std::endl;
    }

    void RBLightingPass::createGraphicsPipelines(const VkGraphicsPipelineCreateInfo &pipelineInfo)
    {
        RBPipelineManager::createGraphicsPipelines(pipelineInfo);
        std::cout << "RBLightingPass::createGraphicsPipelines()" << std::endl;
    }

    void RBLightingPass::fillGraphicsPipelineCreateInfo(uint32_t stageCount,
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
        RBPipelineManager::fillGraphicsPipelineCreateInfo(stageCount, pStages, pVertexInputState, pInputAssemblyState, pTessellationState, pViewportState, pRasterizationState, pMultisampleState, pDepthStencilState, pColorBlendState, pDynamicState, layout, renderPass, subpass, basePipelineHandle, basePipelineIndex);
        std::cout << "RBLightingPass::fillGraphicsPipelineCreateInfo()" << std::endl;
    }

    void RBLightingPass::createGraphicsPipeline()
    {
        setupShaders();

        setupPipelineStates();

        rbPipelineLayoutManager.fillPipelineLayoutInfo(&rbDescriptors.descriptorSetManager.descriptorSetLayoutManager.descriptorSetLayout);
        rbPipelineLayoutManager.createPipelineLayout();

        fillGraphicsPipelineCreateInfo(2, shaderStageInfos.data(), &vertexInputInfo, &inputAssembly, nullptr, &viewportState, &rasterizer, &multisampling, &depthStencil, &colorBlending, nullptr, rbPipelineLayoutManager.pipelineLayout, rbSwapChain.renderPass, 0, VK_NULL_HANDLE, -1);

        createGraphicsPipelines(pipelineInfo);

        std::cout << "RBLightingPass::createGraphicsPipeline()" << std::endl;
    }

    RBLightingPass::RBLightingPass(RBDevice &device, RBSwapChain &swapChain, RBDescriptors &descriptors, const RBPipelineConfig &config)
    : RBPipelineManager(device, swapChain, descriptors), rbPipelineConfig(config),
      vertShaderModule(device), fragShaderModule(device)
    {

        std::cout << "RBLightingPass::RBGraphicPipelineManager()" << std::endl;
    }

    void RBLightingPass::InitializeGraphicPipeline()
    {
        createGraphicsPipeline();
        std::cout << "RBLightingPass::InitializeGraphicPipeline()" << std::endl;
    }

    RBLightingPass::~RBLightingPass()
    {
        vkDestroyDescriptorSetLayout(rbDevice.device, rbDescriptors.descriptorSetManager.descriptorSetLayoutManager.descriptorSetLayout, nullptr);
        std::cout << "RBLightingPass::~RBLightingPass()" << std::endl;
    }
}
