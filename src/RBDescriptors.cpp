//
// Created by rottenbamboo on 2023/6/15.
//

#pragma once

#include <stb_image.h>
#include "RBCommon.h"
#include "RBDescriptors.h"

namespace RottenBamboo{

    void RBDescriptors::createDescriptorSetLayout()
    {
        descriptorSetManager.descriptorSetLayoutManager.fillDescriptorSetLayoutBinding(0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, nullptr);

        for(int i = 0; i < imageCount; i++)
        {
            descriptorSetManager.descriptorSetLayoutManager.fillDescriptorSetLayoutBinding(i + 1, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr);
        }
        //descriptorSetManager.descriptorSetLayoutManager.fillDescriptorSetLayoutBinding(1, ImageCount, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr);
        //descriptorSetManager.descriptorSetLayoutManager.fillDescriptorSetLayoutBinding(1, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr);
        descriptorSetManager.descriptorSetLayoutManager.createDescriptorSetLayout();

   
        if (descriptorSetManager.descriptorSetLayoutManager.descriptorSetLayout == VK_NULL_HANDLE) {
            throw std::runtime_error("Failed to create DescriptorSetLayout!");
        }
    }

     std::pair<uint32_t, uint32_t> RBDescriptors::GetResourceCount() const
    {
        return {this->bufferCount, this->imageCount};
    }
    
    void RBDescriptors::SetDepthAttachmentEnabled(bool depthEnabled)
    {
        enabledDepthAttachment = depthEnabled;
    }

    void RBDescriptors::SetResourceCount(const RBShaderReflection* resourceShaderVertex, const RBShaderReflection* resourceShaderFragment, bool depthEnabled)
    {
        SetDepthAttachmentEnabled(depthEnabled);
        uint32_t depthCount = depthEnabled ? 1 : 0;

        uint32_t bufferCountFragment = resourceShaderFragment->GetUniformBufferCount(0);
        uint32_t bufferCountVertex = resourceShaderVertex->GetUniformBufferCount(0);
        this->bufferCount = std::max(static_cast<uint32_t>(1), std::max(bufferCountFragment, bufferCountVertex));
        
        uint32_t imageCountFragment = resourceShaderFragment->GetCombinedImageSamplerCount(0);
        uint32_t imageCountVertex = resourceShaderVertex->GetCombinedImageSamplerCount(0);
        this->imageCount = imageCountFragment + imageCountVertex + depthCount;

        std::cout << "SetResourceCount()" << ", Buffer count: " << this->bufferCount << ", Image count: " << this->imageCount << std::endl;
    }

    void RBDescriptors::createDescriptorPool()
    {
        descriptorSetManager.descriptorPoolManager.fillDescriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * bufferCount));
        descriptorSetManager.descriptorPoolManager.fillDescriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * imageCount));
        descriptorSetManager.descriptorPoolManager.fillDescriptorPoolCreateInfo(VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT));
        descriptorSetManager.descriptorPoolManager.CreateDescriptorPool();
    }

    void RBDescriptors::createDescriptorSets()
    {
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetManager.descriptorSetLayoutManager.descriptorSetLayout);
        descriptorSetManager.fillDescriptorSetsAllocateInfo(descriptorSetManager.descriptorPoolManager.descriptorPool, MAX_FRAMES_IN_FLIGHT, layouts.data());
        descriptorSetManager.allocateDescriptorSets(rbDevice, MAX_FRAMES_IN_FLIGHT);

        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = rbBufferPtr[i].buffer;
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferShaderVariables);

            // VkDescriptorImageInfo imageInfo{};
            // imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            // imageInfo.imageView = rbImageManager.imageBundles[0].imageView;
            // imageInfo.sampler = rbImageManager.imageBundles[0].sampler;
            descriptorSetManager.fillDescriptotSetsWriteBuffer(i, 0, 0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &bufferInfo);

            for(int j = 0; j < imageCount; j++)
            {
                rbImageManager.imageBundles[j].imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                rbImageManager.imageBundles[j].imageInfo.imageView = rbImageManager.imageBundles[j].imageView;
                rbImageManager.imageBundles[j].imageInfo.sampler = rbImageManager.imageBundles[j].sampler;
                descriptorSetManager.fillDescriptotSetsWriteImage(i, j + 1, 0, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &rbImageManager.imageBundles[j].imageInfo);
            }

            //descriptorSetManager.fillDescriptotSetsWriteImage(i, 1, 0, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &imageInfo);
            descriptorSetManager.updateDescriptorSets(rbDevice);
        }
    }

    void RBDescriptors::createDescriptorSetsFrameBuffer(const TextureParams& attachmentParams, const TextureParams& depthParams)
    {
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetManager.descriptorSetLayoutManager.descriptorSetLayout);
        descriptorSetManager.fillDescriptorSetsAllocateInfo(descriptorSetManager.descriptorPoolManager.descriptorPool, MAX_FRAMES_IN_FLIGHT, layouts.data());
        descriptorSetManager.allocateDescriptorSets(rbDevice, MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = rbBufferPtr[i].buffer;
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferShaderVariables);

            // VkDescriptorImageInfo imageInfo{};
            // imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            // imageInfo.imageView = rbImageManager.imageBundles[0].imageView;
            // imageInfo.sampler = rbImageManager.imageBundles[0].sampler;
            descriptorSetManager.fillDescriptotSetsWriteBuffer(i, 0, 0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &bufferInfo);

            for (int j = 0; j < imageCount; j++)
            {
                VkImageLayout layout = DepthEnabled() && (j == imageCount - 1) ? depthParams.layout : attachmentParams.layout;
                rbImageManager.imageBundles[j].imageInfo.imageLayout = layout;
                rbImageManager.imageBundles[j].imageInfo.imageView = rbImageManager.imageBundles[j].imageView;
                rbImageManager.imageBundles[j].imageInfo.sampler = rbImageManager.imageBundles[j].sampler;
                descriptorSetManager.fillDescriptotSetsWriteImage(i, j + 1, 0, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &rbImageManager.imageBundles[j].imageInfo);
            }

            // descriptorSetManager.fillDescriptotSetsWriteImage(i, 1, 0, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &imageInfo);
            descriptorSetManager.updateDescriptorSets(rbDevice);
        }
    }

#ifdef __ANDROID__
    bool RBDescriptors:: LoadFromMemoryAndroid(const std::string& filePath, bool isHDR, std::vector<uint8_t>& buffer) {
    
    SDL_IOStream* file = SDL_IOFromFile(filePath.c_str(), "rb");
    if (!file) {
        __android_log_print(ANDROID_LOG_ERROR, "RottenBamboo", 
                       "Failed to open texture file: %s, error: %s", 
                       filePath.c_str(), SDL_GetError());
        return false;
    }
    
    Sint64 fileSize = SDL_GetIOSize(file);
    if (fileSize <= 0) {
        __android_log_print(ANDROID_LOG_ERROR, "RottenBamboo", 
                       "Invalid file size: %s", filePath.c_str());
        return false;
    }
    
    buffer.resize(fileSize);
    size_t bytesRead = SDL_ReadIO(file, buffer.data(), fileSize);

    SDL_CloseIO(file);

    if (bytesRead != static_cast<size_t>(fileSize)) {
        __android_log_print(ANDROID_LOG_ERROR, "RottenBamboo", 
                       "Failed to read texture file: %s", filePath.c_str());
        buffer.clear();
        return false;
    }
    return true;
}
#endif
    void RBDescriptors::createTextureImage()
    {
        int index = 0;
        for (auto & imageBundle : rbImageManager.imageBundles)
        {
            int texWidth, texHeight, texChannels, typeSize;
            void* pixels = nullptr;
#ifdef __ANDROID__
            std::vector<uint8_t> buffer;
            LoadFromMemoryAndroid(imagesInfo[index].path, imagesInfo[index].isHDR, buffer);
            // load texture form memory
            if(imagesInfo[index].isHDR)
            {
                pixels = (float*)stbi_loadf_from_memory(buffer.data(), buffer.size(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
                typeSize = sizeof(float);
            }
            else
            {
                pixels = (stbi_uc*)stbi_load_from_memory(buffer.data(), buffer.size(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
                typeSize = sizeof(stbi_uc);
            }
#else
            if(imagesInfo[index].isHDR)
            {
                pixels = (float*)stbi_loadf(imagesInfo[index].path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
                typeSize = sizeof(float);
            }
            else
            {
                pixels = (stbi_uc*)stbi_load(imagesInfo[index].path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
                typeSize = sizeof(stbi_uc);
            }

#endif
            mipLevels = 1;//static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
            mipLevels = 1;//std::min(mipLevels, (uint32_t)8);
            VkDeviceSize imageSize = texWidth * texHeight * 4 * typeSize;
    
            if (!pixels) {
                throw std::runtime_error("failed to load texture image!");
            }
    
            RBBufferManager stageBufferManager(rbDevice);
            stageBufferManager.CreateBufferAllocBindMemory(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    
            void* data = nullptr;
            if(imagesInfo[index].isHDR)
            {
                stageBufferManager.copyMemory(imageSize, data, (float*)pixels);
            }
            else
            {
                stageBufferManager.copyMemory(imageSize, data, (stbi_uc*)pixels);
            }
            stbi_image_free(pixels);


            VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            if(isColorAttachment)
            {
                usageFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            }
            rbImageManager.fillImageInfo(texWidth, texHeight, mipLevels, msaaSamples, imagesInfo[index].format, VK_IMAGE_TILING_OPTIMAL, usageFlags);
            rbImageManager.createImage(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, imageBundle.image, imageBundle.imageMemory);

            VkCommandBuffer commandBuffer = rbCommandBuffer.beginSingleTimeCommands(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
            rbImageManager.transitionImageLayout(commandBuffer, imageBundle.image, imagesInfo[index].format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);
            rbCommandBuffer.endSingleTimeCommands(commandBuffer);

            copyBufferToImage(stageBufferManager.buffer, imageBundle.image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

            //VkCommandBuffer commandBufferEnd = rbCommandBuffer.beginSingleTimeCommands(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
            //rbImageManager.transitionImageLayout(commandBufferEnd, imageBundle.image, imagesInfo[index].format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, mipLevels);
            //rbCommandBuffer.endSingleTimeCommands(commandBufferEnd);

            VkFormatProperties formatProperties;
            vkGetPhysicalDeviceFormatProperties(rbDevice.physicalDevice, imagesInfo[index].format, &formatProperties);
            
            bool supportsLinearBlitting = (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT) != 0;
            bool supportsBlit = (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT) != 0 && 
                               (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT) != 0;
            
            if (supportsLinearBlitting && supportsBlit && mipLevels > 1) 
            {
                generateMipmaps(imageBundle.image, imagesInfo[index].format, texWidth, texHeight, mipLevels);
            } 
            else 
            {
                // transition to final layout directly
                VkCommandBuffer layoutCmdBuffer = rbCommandBuffer.beginSingleTimeCommands(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
                rbImageManager.transitionImageLayout(layoutCmdBuffer, imageBundle.image, imagesInfo[index].format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, mipLevels);
                rbCommandBuffer.endSingleTimeCommands(layoutCmdBuffer);
            }
            index++;
            std::cout << "index = " << index << std::endl;
            std::cout << "mipLevels = " << mipLevels << std::endl;
        }
    }

    void RBDescriptors::createTextureImageFrameBuffer(VkExtent2D framebufferExtent, 
                                                      const TextureParams& attachmentParams,
                                                      const TextureParams& depthParams)
    {
        int imageIndex = 0;
        for (auto & imageBundle : rbImageManager.imageBundles)
        {
            int texWidth = framebufferExtent.width;
            int texHeight = framebufferExtent.height;
            VkDeviceSize imageSize = texWidth * texHeight * 4;

            // VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            // if(isColorAttachment)
            // {
            //     usageFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            // }

            VkImageUsageFlagBits usageFlags = DepthEnabled() && (imageIndex == imageCount - 1) ? depthParams.usage : attachmentParams.usage;
            VkFormat format = DepthEnabled() && (imageIndex == imageCount - 1) ? depthParams.format : attachmentParams.format;
            rbImageManager.fillImageInfo(texWidth, texHeight, mipLevels, msaaSamples, format, VK_IMAGE_TILING_OPTIMAL, usageFlags);
            rbImageManager.createImage(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, imageBundle.image, imageBundle.imageMemory);

            imageIndex++;
            std::cout << "index = " << imageIndex << std::endl;
            std::cout << "mipLevels = " << mipLevels << std::endl;
        }
    }

    void RBDescriptors::setTextureImageCount()
    {
        rbImageManager.setImageCount(this->imageCount);
    }

    void RBDescriptors::createTextureImageView()
    {
        int index = 0;
        for (auto & imageBundle : rbImageManager.imageBundles)
        {
            rbImageManager.fillViewInfo(imageBundle.viewInfo, imageBundle.image, VK_IMAGE_VIEW_TYPE_2D, imagesInfo[index].format, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels);
            rbImageManager.createImageView(imageBundle.viewInfo, imageBundle.imageView);
            index++;
        }

    }

    void RBDescriptors::createTextureImageViewFrameBuffer(const TextureParams& attachmentParams,
                                                          const TextureParams& depthParams)
    {
        int imageViewIndex = 0;
        for (auto & imageBundle : rbImageManager.imageBundles)
        {
            VkFormat format = (DepthEnabled() && (imageViewIndex == imageCount - 1)) ? depthParams.format : attachmentParams.format;
            VkImageAspectFlagBits aspect = (DepthEnabled() && (imageViewIndex == imageCount - 1)) ? depthParams.aspect : attachmentParams.aspect;
            rbImageManager.fillViewInfo(imageBundle.viewInfo, imageBundle.image, VK_IMAGE_VIEW_TYPE_2D, format, aspect, mipLevels);
            rbImageManager.createImageView(imageBundle.viewInfo, imageBundle.imageView);
            imageViewIndex++;
        }

    }

    void RBDescriptors::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
        VkCommandBuffer commandBuffer = rbCommandBuffer.beginSingleTimeCommands(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

        rbImageManager.copyBufferToImage(commandBuffer, buffer, image, width, height);

        rbCommandBuffer.endSingleTimeCommands(commandBuffer);
    }

    void RBDescriptors::generateMipmaps(VkImage image, VkFormat imageFormat, uint32_t texWidth, uint32_t texHeight, uint32_t mipLevels)
    {
        // check linear blitting compatibility
        VkFormatProperties formatProperties;
        vkGetPhysicalDeviceFormatProperties(rbDevice.physicalDevice, imageFormat, &formatProperties);

        if(!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
        {
            throw std::runtime_error("texture image format does not support linear blitting!");
        }

        bool supportsLinearBlitting = (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT) != 0;
        bool supportsBlit = (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT) != 0 && 
                           (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT) != 0;

        if (!supportsLinearBlitting || !supportsBlit) {
        
            // transition to final layout directly
        VkCommandBuffer commandBuffer = rbCommandBuffer.beginSingleTimeCommands(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
            
            VkImageMemoryBarrier barrier{};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.image = image;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.levelCount = mipLevels;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;
            
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            vkCmdPipelineBarrier(commandBuffer, 
                                VK_PIPELINE_STAGE_TRANSFER_BIT, 
                                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 
                                0, 0, nullptr, 0, nullptr, 1, &barrier);
            
            rbCommandBuffer.endSingleTimeCommands(commandBuffer);
            
            throw std::runtime_error("texture image format does not support linear blitting!");
        }

        // desktop code for generating mipmaps
        VkCommandBuffer commandBuffer = rbCommandBuffer.beginSingleTimeCommands(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.image = image;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.subresourceRange.levelCount = 1;

        int32_t mipWidth = texWidth;
        int32_t mipHeight = texHeight;
        for(uint32_t i = 1; i < mipLevels; i++)
        {
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

            VkImageBlit blit{};
            blit.srcOffsets[0] = {0, 0, 0};
            blit.srcOffsets[1] = { mipWidth, mipHeight, 1};
            blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.srcSubresource.mipLevel = i - 1;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount = 1;
            blit.dstOffsets[0] = {0, 0, 0};
            blit.dstOffsets[1] = {mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1};
            blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.dstSubresource.mipLevel = i;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 1;

            vkCmdBlitImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

            if(mipWidth > 1) mipWidth /= 2;
            if(mipHeight > 1) mipHeight /= 2;
        }

        barrier.subresourceRange.baseMipLevel = mipLevels - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        rbCommandBuffer.endSingleTimeCommands(commandBuffer);
    }

    void RBDescriptors::createTextureSampler()
    {
        rbImageManager.fillSampler(VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_COMPARE_OP_LESS_OR_EQUAL, mipLevels, imageCount);
        rbImageManager.createTextureSampler();
    }

    void RBDescriptors::createTextureSamplerFrameBuffer()
    {
        rbImageManager.fillSampler(VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_COMPARE_OP_LESS_OR_EQUAL, mipLevels, imageCount);
        rbImageManager.createTextureSampler();
    }

    RBDescriptors::RBDescriptors(RBDevice& device, RBCommandBuffer& commandBuffer, RBBuffer<UniformBufferShaderVariables> *uniformBuffers, const std::vector<TexturesInfo> &texturesInfo, bool isColorAttachment) : rbDevice(device),
    rbCommandBuffer(commandBuffer),
    rbBufferPtr(uniformBuffers),
    descriptorSetManager(device),
    rbImageManager(device),
    isColorAttachment(isColorAttachment),
    imagesInfo(texturesInfo)
    {
    }

    RBDescriptors::RBDescriptors(RBDevice& device, RBCommandBuffer& commandBuffer, RBBuffer<UniformBufferShaderVariables> *uniformBuffers, bool isColorAttachment) : rbDevice(device),
    rbCommandBuffer(commandBuffer),
    rbBufferPtr(uniformBuffers),
    descriptorSetManager(device),
    rbImageManager(device),
    isColorAttachment(isColorAttachment)
    {
    }

    void RBDescriptors::InitializeDescriptors(const RBShaderReflection* resourceShaderVertex, const RBShaderReflection* resourceShaderFragment, bool depthEnabled)
    {
        //checkImagesInfo();
        for (std::unordered_map<uint32_t, RBDescriptorSetLayoutInfo>::const_iterator descriptorSetIt = resourceShaderVertex->descriptorSets.begin();
             descriptorSetIt != resourceShaderVertex->descriptorSets.end(); ++descriptorSetIt)
        {
            uint32_t setIndex = descriptorSetIt->first;
            const RBDescriptorSetLayoutInfo& descriptorSetLayoutInfo = descriptorSetIt->second;
            for(std::unordered_map<uint32_t, RBDescriptorBinding>::const_iterator bindingIt = descriptorSetLayoutInfo.bindings.begin();
                bindingIt != descriptorSetLayoutInfo.bindings.end(); ++bindingIt)
            {
                const RBDescriptorBinding& descriptorBinding = bindingIt->second;
            }
        }
        
        SetResourceCount(resourceShaderVertex, resourceShaderFragment, depthEnabled);
        setTextureImageCount();
        createTextureImage();
        createTextureImageView();
        createTextureSampler();
        createDescriptorPool();
        createDescriptorSetLayout();
        createDescriptorSets();
    }

    void RBDescriptors::checkImagesInfo()
    {
        // if(imagesInfo.size() != ImageCount)
        // {
        //     throw std::runtime_error("imagesInfo size does not match ImageCount!");
        // }
        // for(int i = 0; i < imagesInfo.size(); i++)
        // { 
        //     if(!checkFormatSupported(rbDevice.deviceSwapChainSupport.formats, imagesInfo[i].format))
        //     {
        //         imagesInfo[i].format = fallBackFormat.format;
        //         imagesInfo[i].isHDR = fallBackFormat.isHDR;
        //     }
        // }
    }

    void RBDescriptors::InitializeDescriptorsFrameBuffer(VkExtent2D framebufferExtent, 
                                                                                  const TextureParams& attachmentParams,
                                                                                  const TextureParams& depthParams,
                                                                                  const RBShaderReflection* resourceShaderVertex, 
                                                                                  const RBShaderReflection* resourceShaderFragment, bool depthEnabled)
    {
        SetResourceCount(resourceShaderVertex, resourceShaderFragment, depthEnabled);
        setTextureImageCount();
        createTextureImageFrameBuffer(framebufferExtent, attachmentParams, depthParams);
        createTextureImageViewFrameBuffer(attachmentParams, depthParams);
        createTextureSamplerFrameBuffer();
        createDescriptorPool();
        createDescriptorSetLayout();
        createDescriptorSetsFrameBuffer(attachmentParams, depthParams);
    }

    void RBDescriptors::ReleaseAllResource()
    {
        rbImageManager.ReleaseAllResource();
        descriptorSetManager.Destroy();
    }

    RBDescriptors::~RBDescriptors()
    {
    }
}
