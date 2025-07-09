//
// Created by rottenbamboo on 2023/5/22.
//

#include "RBPipelineUtils.h"
#include "RBLightingPass.h"

namespace RottenBamboo {

    void RBLightingPass::setupShaders()
    {
        fillShaderModule("../shader/bin/lightingVert.spv", VK_SHADER_STAGE_VERTEX_BIT, "main", vertShaderModule);
        fillShaderModule("../shader/bin/lightingFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT, "main", fragShaderModule);
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
    void RBLightingPass::createFrameBuffers() 
    {
 
    }

    void RBLightingPass::setupAttachments()
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(rbDevice.physicalDevice, rbDevice.surface);

        VkSurfaceFormatKHR surfaceFormat;// = chooseSwapSurfaceFormat(swapChainSupport.formats);
        surfaceFormat.format = VK_FORMAT_R16G16B16A16_SFLOAT;
        //surfaceFormat.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

        fillColorAttachment(surfaceFormat.format, msaaSamples, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

        fillColorResolveAttachment(surfaceFormat.format, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

        fillDepthAttachment(findDepthFormat(), msaaSamples, VK_ATTACHMENT_LOAD_OP_LOAD, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        std::cout << "RBLightingPass::setupAttachments()" << std::endl;
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
                                        uint32_t subpass,
                                        VkPipeline basePipelineHandle,
                                        int32_t basePipelineIndex
                                        )
    {
        RBPipelineManager::fillGraphicsPipelineCreateInfo(stageCount, pStages, pVertexInputState, pInputAssemblyState, pTessellationState, pViewportState, pRasterizationState, pMultisampleState, pDepthStencilState, pColorBlendState, pDynamicState, layout, subpass, basePipelineHandle, basePipelineIndex);
        std::cout << "RBLightingPass::fillGraphicsPipelineCreateInfo()" << std::endl;
    }

    void RBLightingPass::createGraphicsPipeline()
    {
        setupShaders();

        setupPipelineStates();

        rbPipelineLayoutManager.fillPipelineLayoutInfo(&rbDescriptors.descriptorSetManager.descriptorSetLayoutManager.descriptorSetLayout);
        rbPipelineLayoutManager.createPipelineLayout();

        fillGraphicsPipelineCreateInfo(2, shaderStageInfos.data(), &vertexInputInfo, &inputAssembly, nullptr, &viewportState, &rasterizer, &multisampling, &depthStencil, &colorBlending, nullptr, rbPipelineLayoutManager.pipelineLayout, 0, VK_NULL_HANDLE, -1);

        createGraphicsPipelines(pipelineInfo);

        std::cout << "RBLightingPass::createGraphicsPipeline()" << std::endl;
    }

    RBLightingPass::RBLightingPass(int colorAttachmentCount, bool bResolveAttachment, bool bDephAttament, RBDevice &device, RBDescriptors<TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT, 1> &descriptors, const RBPipelineConfig &config, VkImageLayout layout)
    : RBPipelineManager(colorAttachmentCount, bResolveAttachment, bDephAttament, device, layout), rbPipelineConfig(config),
      vertShaderModule(device), fragShaderModule(device), rbDescriptors(descriptors)
    {

        std::cout << "RBLightingPass::RBGraphicPipelineManager()" << std::endl;
    }

    void RBLightingPass::InitializeGraphicPipeline()
    {
        createGraphicsPipeline();
        std::cout << "RBLightingPass::InitializeGraphicPipeline()" << std::endl;
    }
    void RBLightingPass::fillDepthStencilStateCreateInfo()
    {
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.flags = 0;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {};
        depthStencil.back = {};
        depthStencil.minDepthBounds = 0.0f;
        depthStencil.maxDepthBounds = 1.0f;
        std::cout << "RBLightingPass::fillDepthStencilStateCreateInfo()" << std::endl;
    }

    RBLightingPass::~RBLightingPass()
    {
        vkDestroyDescriptorSetLayout(rbDevice.device, rbDescriptors.descriptorSetManager.descriptorSetLayoutManager.descriptorSetLayout, nullptr);
        std::cout << "RBLightingPass::~RBLightingPass()" << std::endl;
    }
    void RBLightingPass::recordCommandBuffer(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo renderPassInfo, RBDescriptors<TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT, 1>& descriptors, RBMesh &mesh, RBGUI& gui, UniformBufferShaderVariables& uniformMatirx) 
    {
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rbPipelineLayoutManager.pipelineLayout, 0, 1, &descriptors.descriptorSetManager.descriptorSets[currentFrame], 0, nullptr);

        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

        gui.Render(commandBuffer, uniformMatirx);

        vkCmdEndRenderPass(commandBuffer);
    }
}
