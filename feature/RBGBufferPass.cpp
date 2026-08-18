//
// Created by rottenbamboo on 2023/5/22.
//
#pragma once
#include "RBPipelineUtils.h"
#include "RBGBufferPass.h"

namespace RottenBamboo {

    void RBGBufferPass::setupShaders()
    {
        shaderStageInfos.clear();
        auto shaderCode = RBPipelineUtils::readFile(GET_PROJECT_ROOT_DIR + "shader/bin/gBufferVert.spv");
        //RBPipelineUtils::ReflectShader(reinterpret_cast<const uint32_t*>(shaderCode.data()), shaderCode.size() / sizeof(uint32_t));
        fillShaderModule(GET_PROJECT_ROOT_DIR + "shader/bin/gBufferVert.spv", VK_SHADER_STAGE_VERTEX_BIT, "main", vertShaderModule);
        fillShaderModule(GET_PROJECT_ROOT_DIR + "shader/bin/gBufferFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT, "main", fragShaderModule);
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
        int attachmentCount = rbColorAttachmentDescriptors.rbImageManager.imageBundles.size();
        std::vector<VkImageView> attachments;
        attachments.reserve(attachmentCount);
        
        std::cout << "gBufferFrameBuffers.resize(" << MAX_FRAMES_IN_FLIGHT << ")"<< std::endl;
        
        for (int i = 0; i < attachmentCount; ++i) {
            attachments.push_back(rbColorAttachmentDescriptors.rbImageManager.imageBundles[i].imageView); // GBuffer colorAttachment imageView
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
            RBLOG_FATAL("failed to create GBuffer framebuffer!");
            throw std::runtime_error("failed to create GBuffer framebuffer!");
        }
    }

    void RBGBufferPass::setResourceCount()
    {
        isDepthAttachment = rbColorAttachmentDescriptors.DepthEnabled() ? true : false;
        rbColorAttachmentCount = rbColorAttachmentDescriptors.rbImageManager.getImageCount();
        pureColorAttachmentCount = rbColorAttachmentCount - (isDepthAttachment ? 1 : 0);
        depthAttachmentCount = isDepthAttachment ? 1 : 0;
        ColorAttachKind = 1 + (isResolveAttachment ? 1 : 0);
    }

    void RBGBufferPass::setupAttachments()
    {
        VkSurfaceFormatKHR surfaceFormat;// = chooseSwapSurfaceFormat(swapChainSupport.formats);
        surfaceFormat.format = VK_FORMAT_R8G8B8A8_UNORM;
        surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

        fillColorAttachment(surfaceFormat.format, msaaSamples, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        fillColorResolveAttachment(surfaceFormat.format, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

        fillDepthAttachment(findDepthFormat(), msaaSamples, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED,VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

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

    RBGBufferPass::RBGBufferPass(int colorAttachmentCount, bool bResolveAttachment, bool bDephAttament, RBDevice &device, RBDescriptors &descriptors, RBDescriptors &descriptorColorAttachment, const RBPipelineConfig &config, VkImageLayout layout)
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

    void RBGBufferPass::Execute(VkCommandBuffer commandBuffer, VkRenderPassBeginInfo renderPassInfo, std::vector<RBDescriptors*> pDescriptorsGBuffersVec, ResourceManager& resourceManager) 
    {
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

        
        for(auto it = model_paths.cbegin(); it != model_paths.cend(); ++it)
        {
            auto shared_ptr_model = resourceManager.Get<RBModel>(it->second);
            auto& mesh = shared_ptr_model->getMeshes(0);
            VkBuffer vertexBuffers[] = {(*mesh).vertexBuffer.buffer};
            VkDeviceSize offsets[] = {0};

            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rbPipelineLayoutManager.pipelineLayout, 0, 1, &pDescriptorsGBuffersVec[it->first]->descriptorSetManager.descriptorSets[currentFrame], 0, nullptr);
        
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

            vkCmdBindIndexBuffer(commandBuffer, (*mesh).indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);

            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>((*mesh).indexBuffer.data.size()), 1, 0, 0, 0);
        }

        vkCmdEndRenderPass(commandBuffer);
    }

}
