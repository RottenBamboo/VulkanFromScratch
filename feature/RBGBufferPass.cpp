//
// Created by rottenbamboo on 2023/5/22.
//

#include "RBPipelineUtils.h"
#include "RBGBufferPass.h"

namespace RottenBamboo {

    void RBGBufferPass::setupShaders()
    {
        fillShaderModule("../shader/bin/gBufferVert.spv", VK_SHADER_STAGE_VERTEX_BIT, "main", vertShaderModule);
        fillShaderModule("../shader/bin/gBufferFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT, "main", fragShaderModule);
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


    void RBGBufferPass::clearFrameBuffers() 
    {
        vkDestroyFramebuffer(rbDevice.device, gBufferFrameBuffers, nullptr);
        gBufferFrameBuffers = VK_NULL_HANDLE;
        std::cout << "RBGBufferPass::refreFrameBuffers()" << std::endl;
    }
    void RBGBufferPass::createFrameBuffers() {
        std::vector<VkImageView> attachments(rbColorAttachmentCount);
        
        std::cout << "gBufferFrameBuffers.resize(" << MAX_FRAMES_IN_FLIGHT << ")"<< std::endl;
        
        for (int i = 0; i < rbColorAttachmentCount; ++i) {
            attachments[i] = rbColorAttachmentDescriptors.rbImageManager.imageBundles[i].imageView; // GBuffer colorAttachment imageView
        }

        
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass; // GBufferPass RenderPass
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;
    
        if (vkCreateFramebuffer(rbDevice.device, &framebufferInfo, nullptr, &gBufferFrameBuffers) != VK_SUCCESS) {
            throw std::runtime_error("failed to create GBuffer framebuffer!");
        }
    }

    void RBGBufferPass::setupAttachments()
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(rbDevice.physicalDevice, rbDevice.surface);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);

        fillColorAttachment(surfaceFormat.format, msaaSamples, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

        fillColorResolveAttachment(surfaceFormat.format, msaaSamples, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

        fillDepthAttachment(findDepthFormat(), msaaSamples, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        std::cout << "RBGBufferPass::setupAttachments()" << std::endl;
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

    RBGBufferPass::RBGBufferPass(int colorAttachmentCount, bool bResolveAttachment, bool bDephAttament, RBDevice &device, RBDescriptors<TEXTURE_PATHS_MECH_COUNT, 1> &descriptors, RBDescriptors<TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT, 1> &descriptorColorAttachment, const RBPipelineConfig &config, VkImageLayout layout)
        : RBPipelineManager(colorAttachmentCount, bResolveAttachment, bDephAttament, device, layout), rbPipelineConfig(config),
        vertShaderModule(device), fragShaderModule(device), rbDescriptors(descriptors), rbColorAttachmentDescriptors(descriptorColorAttachment)
    {
        std::cout << "RBGBufferPass::RBGBufferPass()" << std::endl;
    }

    void RBGBufferPass::InitializeGraphicPipeline()
    {
        createGraphicsPipeline();
        std::cout << "RBGBufferPass::InitializeGraphicPipeline()" << std::endl;
    }
    void RBGBufferPass::fillDepthStencilStateCreateInfo()
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
        std::cout << "RBGBufferPass::fillDepthStencilStateCreateInfo()" << std::endl;
    }

    RBGBufferPass::~RBGBufferPass()
    {
        //vkDestroyDescriptorSetLayout(rbDevice.device, rbDescriptors.descriptorSetManager.descriptorSetLayoutManager.descriptorSetLayout, nullptr);
        //vkDestroyDescriptorSetLayout(rbDevice.device, rbColorAttachmentDescriptors.descriptorSetManager.descriptorSetLayoutManager.descriptorSetLayout, nullptr);
        vkDestroyFramebuffer(rbDevice.device, gBufferFrameBuffers, nullptr);
        std::cout << "RBGBufferPass::~RBGBufferPass()" << std::endl;
    }

    void RBGBufferPass::recordCommandBuffer(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo renderPassInfo, RBDescriptors<TEXTURE_PATHS_MECH_COUNT, 1>& descriptorsGBuffer, RBMesh &mesh) 
    {
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rbPipelineLayoutManager.pipelineLayout, 0, 1, &descriptorsGBuffer.descriptorSetManager.descriptorSets[currentFrame], 0, nullptr);

        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh.indexBuffer.data.size()), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffer);
    }

}
