//
// Created by rottenbamboo on 2023/5/22.
//

#include "RBPipelineUtils.h"
#include "RBGraphicPipelineManager.h"

namespace RottenBamboo {

    void RBGraphicPipelineManager::setupShaders()
    {
        fillShaderModule("../shader/vert.spv", VK_SHADER_STAGE_VERTEX_BIT, "main");
        fillShaderModule("../shader/frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT, "main");
    }

    void RBGraphicPipelineManager::fillShaderModule(const std::string& shaderName, VkShaderStageFlagBits stage, const char* pName)
    {
        auto shaderCode = RBPipelineUtils::readFile(shaderName);
        VkPipelineShaderStageCreateInfo shaderStageInfo{};

        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.pNext = nullptr;
        shaderStageInfo.stage = stage;
        shaderStageInfo.module = RBPipelineUtils::createShaderModule(rbDevice, shaderCode);
        shaderStageInfo.pName = "main";
        shaderStageInfos.push_back(shaderStageInfo);
        std::cout << "RBGraphicPipelineManager::~fillShaderModule()" << std::endl;

    }

    void RBGraphicPipelineManager::setupPipelineStates()
    {
        RBPipelineManager::setupPipelineStates();
        std::cout << "RBGraphicPipelineManager::setupPipelineStates()" << std::endl;
    }

    void RBGraphicPipelineManager::createGraphicsPipelines(const VkGraphicsPipelineCreateInfo &pipelineInfo)
    {
        RBPipelineManager::createGraphicsPipelines(pipelineInfo);
        std::cout << "RBGraphicPipelineManager::createGraphicsPipelines()" << std::endl;
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
        RBPipelineManager::fillGraphicsPipelineCreateInfo(stageCount, pStages, pVertexInputState, pInputAssemblyState, pTessellationState, pViewportState, pRasterizationState, pMultisampleState, pDepthStencilState, pColorBlendState, pDynamicState, layout, renderPass, subpass, basePipelineHandle, basePipelineIndex);
        std::cout << "RBGraphicPipelineManager::fillGraphicsPipelineCreateInfo()" << std::endl;
    }

    void RBGraphicPipelineManager::createGraphicsPipeline()
    {
        setupShaders();

        setupPipelineStates();

        rbPipelineLayoutManager.fillPipelineLayoutInfo(&rbDescriptors.descriptorSetManager.descriptorSetLayoutManager.descriptorSetLayout);
        rbPipelineLayoutManager.createPipelineLayout();

        fillGraphicsPipelineCreateInfo(2, shaderStageInfos.data(), &vertexInputInfo, &inputAssembly, nullptr, &viewportState, &rasterizer, &multisampling, &depthStencil, &colorBlending, nullptr, rbPipelineLayoutManager.pipelineLayout, rbSwapChain.renderPass, 0, VK_NULL_HANDLE, -1);

        createGraphicsPipelines(pipelineInfo);

        vkDestroyShaderModule(rbDevice.device, vertShaderModule, nullptr);
        vkDestroyShaderModule(rbDevice.device, fragShaderModule, nullptr);

        std::cout << "RBGraphicPipelineManager::createGraphicsPipeline()" << std::endl;
    }

    RBGraphicPipelineManager::RBGraphicPipelineManager(RBDevice &device, RBSwapChain &swapChain, RBDescriptors &descriptors, const RBPipelineConfig &config)
    : RBPipelineManager(device, swapChain, descriptors), rbPipelineConfig(config)
    {

        std::cout << "RBGraphicPipelineManager::RBGraphicPipelineManager()" << std::endl;
    }

    void RBGraphicPipelineManager::InitializeGraphicPipeline()
    {
        createGraphicsPipeline();
        std::cout << "RBGraphicPipelineManager::InitializeGraphicPipeline()" << std::endl;
    }

    RBGraphicPipelineManager::~RBGraphicPipelineManager()
    {
        for(int i = 0; i < shaderStageInfos.size(); i++)
        {
            vkDestroyShaderModule(rbDevice.device, shaderStageInfos[i].module, nullptr);
        }

        //vkDestroyPipeline(rbDevice.device, graphicsPipeline, nullptr);
        RBPipelineManager::~RBPipelineManager();
        vkDestroyDescriptorSetLayout(rbDevice.device, rbDescriptors.descriptorSetManager.descriptorSetLayoutManager.descriptorSetLayout, nullptr);
        std::cout << "RBGraphicPipelineManager::_RBGraphicPipelineManager()" << std::endl;
    }
}
