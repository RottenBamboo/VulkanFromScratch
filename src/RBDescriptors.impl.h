//
// Created by rottenbamboo on 2023/6/15.
//

#pragma once

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
//#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace RottenBamboo{

    template<int ImageCount, int BufferCount>
    void RBDescriptors<ImageCount, BufferCount>::createDescriptorSetLayout()
    {
        descriptorSetManager.descriptorSetLayoutManager.fillDescriptorSetLayoutBinding(0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, nullptr);

        for(int i = 0; i < ImageCount; i++)
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

    template<int ImageCount, int BufferCount>
    void RBDescriptors<ImageCount, BufferCount>::createDescriptorPool()
    {
        descriptorSetManager.descriptorPoolManager.fillDescriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * BufferCount));
        descriptorSetManager.descriptorPoolManager.fillDescriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * ImageCount));
        descriptorSetManager.descriptorPoolManager.fillDescriptorPoolCreateInfo(VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT));
        descriptorSetManager.descriptorPoolManager.CreateDescriptorPool();
    }

    template<int ImageCount, int BufferCount>
    void RBDescriptors<ImageCount, BufferCount>::createDescriptorSets()
    {
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetManager.descriptorSetLayoutManager.descriptorSetLayout);
        descriptorSetManager.fillDescriptorSetsAllocateInfo(descriptorSetManager.descriptorPoolManager.descriptorPool, MAX_FRAMES_IN_FLIGHT, layouts.data());
        descriptorSetManager.allocateDescriptorSets(rbDevice, MAX_FRAMES_IN_FLIGHT);

        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = rbBufferPtr[i].buffer;
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            // VkDescriptorImageInfo imageInfo{};
            // imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            // imageInfo.imageView = rbImageManager.imageBundles[0].imageView;
            // imageInfo.sampler = rbImageManager.imageBundles[0].sampler;
            descriptorSetManager.fillDescriptotSetsWriteBuffer(i, 0, 0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &bufferInfo);

            for(int j = 0; j < ImageCount; j++)
            {
                rbImageManager.imageBundles[j].imageInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                rbImageManager.imageBundles[j].imageInfo.imageView = rbImageManager.imageBundles[j].imageView;
                rbImageManager.imageBundles[j].imageInfo.sampler = rbImageManager.imageBundles[j].sampler;
                descriptorSetManager.fillDescriptotSetsWriteImage(i, j + 1, 0, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &rbImageManager.imageBundles[j].imageInfo);
            }

            //descriptorSetManager.fillDescriptotSetsWriteImage(i, 1, 0, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &imageInfo);
            descriptorSetManager.updateDescriptorSets(rbDevice);
        }
    }

    template<int ImageCount, int BufferCount>
    void RBDescriptors<ImageCount, BufferCount>::createTextureImage()
    {
        int index = 0;
        for (auto & imageBundle : rbImageManager.imageBundles)
        {
            int texWidth, texHeight, texChannels;
            stbi_uc* pixels = stbi_load(paths[index].c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
            mipLevels = 1;//static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
            mipLevels = 1;//std::min(mipLevels, (uint32_t)8);
            VkDeviceSize imageSize = texWidth * texHeight * 4;
    
            if (!pixels) {
                throw std::runtime_error("failed to load texture image!");
            }
    
            RBBufferManager stageBufferManager(rbDevice);
            stageBufferManager.CreateBufferAllocBindMemory(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    
            void* data = nullptr;
            stageBufferManager.copyMemory(imageSize, data, pixels);
    
            stbi_image_free(pixels);


            VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            if(isColorAttachment)
            {
                usageFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            }
            rbImageManager.fillImageInfo(texWidth, texHeight, mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, usageFlags);
            rbImageManager.createImage(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, imageBundle.image, imageBundle.imageMemory);

            VkCommandBuffer commandBuffer = rbCommandBuffer.beginSingleTimeCommands(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
            rbImageManager.transitionImageLayout(commandBuffer, imageBundle.image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);
            rbCommandBuffer.endSingleTimeCommands(commandBuffer);

            copyBufferToImage(stageBufferManager.buffer, imageBundle.image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

            //VkCommandBuffer commandBufferEnd = rbCommandBuffer.beginSingleTimeCommands(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
            //rbImageManager.transitionImageLayout(commandBufferEnd, imageBundle.image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, mipLevels);
            //rbCommandBuffer.endSingleTimeCommands(commandBufferEnd);

            generateMipmaps(imageBundle.image, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels);
            index++;
            std::cout << "index = " << index << std::endl;
            std::cout << "mipLevels = " << mipLevels << std::endl;
        }
    }

    template<int ImageCount, int BufferCount>
    void RBDescriptors<ImageCount, BufferCount>::createTextureImageFrameBuffer(VkExtent2D framebufferExtent)
    {
        int index = 0;
        for (auto & imageBundle : rbImageManager.imageBundles)
        {
            int texWidth = framebufferExtent.width;
            int texHeight = framebufferExtent.height;
            VkDeviceSize imageSize = texWidth * texHeight * 4;

            VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            if(isColorAttachment)
            {
                usageFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            }
            rbImageManager.fillImageInfo(texWidth, texHeight, mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, usageFlags);
            rbImageManager.createImage(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, imageBundle.image, imageBundle.imageMemory);

            index++;
            std::cout << "index = " << index << std::endl;
            std::cout << "mipLevels = " << mipLevels << std::endl;
        }
    }

    template<int ImageCount, int BufferCount>
    void RBDescriptors<ImageCount, BufferCount>::createTextureImageView()
    {
        for (auto & imageBundle : rbImageManager.imageBundles)
        {
            rbImageManager.fillViewInfo(imageBundle.viewInfo, imageBundle.image, VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels);
            rbImageManager.createImageView(imageBundle.viewInfo, imageBundle.imageView);
        }

    }

    template<int ImageCount, int BufferCount>
    void RBDescriptors<ImageCount, BufferCount>::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
        VkCommandBuffer commandBuffer = rbCommandBuffer.beginSingleTimeCommands(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

        rbImageManager.copyBufferToImage(commandBuffer, buffer, image, width, height);

        rbCommandBuffer.endSingleTimeCommands(commandBuffer);
    }

    template<int ImageCount, int BufferCount>
    void RBDescriptors<ImageCount, BufferCount>::generateMipmaps(VkImage image, VkFormat imageFormat, uint32_t texWidth, uint32_t texHeight, uint32_t mipLevels)
    {
        VkFormatProperties formatProperties;
        vkGetPhysicalDeviceFormatProperties(rbDevice.physicalDevice, imageFormat, &formatProperties);

        if(!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
        {
            throw std::runtime_error("texture image format does not support linear blitting!");
        }

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

    template<int ImageCount, int BufferCount>
    void RBDescriptors<ImageCount, BufferCount>::createTextureSampler()
    {
        rbImageManager.fillSampler(VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_COMPARE_OP_LESS_OR_EQUAL, mipLevels);
        rbImageManager.createTextureSampler();
    }

    template<int ImageCount, int BufferCount>
    RBDescriptors<ImageCount, BufferCount>::RBDescriptors(RBDevice& device, RBCommandBuffer& commandBuffer, RBBuffer<UniformBufferObject> *uniformBuffers, const std::array<std::string, ImageCount> &texturePaths, bool isColorAttachment) : rbDevice(device),
    rbCommandBuffer(commandBuffer),
    rbBufferPtr(uniformBuffers),
    descriptorSetManager(device),
    rbImageManager(device),
    isColorAttachment(isColorAttachment),
    paths{texturePaths}
    {
    }

    template<int ImageCount, int BufferCount>
    void RBDescriptors<ImageCount, BufferCount>::InitializeDescriptors()
    {
        createTextureImage();
        createTextureImageView();
        createTextureSampler();
        createDescriptorPool();
        createDescriptorSetLayout();
        createDescriptorSets();
    }

    template<int ImageCount, int BufferCount>
    void RBDescriptors<ImageCount, BufferCount>::InitializeDescriptorsFrameBuffer(VkExtent2D framebufferExtent)
    {
        createTextureImageFrameBuffer(framebufferExtent);
        createTextureImageView();
        createTextureSampler();
        createDescriptorPool();
        createDescriptorSetLayout();
        createDescriptorSets();
    }

    template<int ImageCount, int BufferCount>
    RBDescriptors<ImageCount, BufferCount>::~RBDescriptors()
    {
    }
}
