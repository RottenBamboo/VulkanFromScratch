//
// Created by rottenbamboo on 2023/6/15.
//

#include "RBDescriptors.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace RottenBamboo{

    void RBDescriptors::createDescriptorSetLayout()
    {
        descriptorSetManager.descriptorSetLayoutManager.fillDescriptorSetLayoutBinding(0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, nullptr);
        descriptorSetManager.descriptorSetLayoutManager.fillDescriptorSetLayoutBinding(1, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr);
        descriptorSetManager.descriptorSetLayoutManager.createDescriptorSetLayout();
    }

    void RBDescriptors::createDescriptorPool()
    {
        descriptorSetManager.descriptorPoolManager.fillDescriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT));
        descriptorSetManager.descriptorPoolManager.fillDescriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT));
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
            bufferInfo.range = sizeof(UniformBufferObject);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = rbImageManager.textureImageView;
            imageInfo.sampler = rbImageManager.textureSampler;

            descriptorSetManager.fillDescriptotSetsWriteBuffer(i, 0, 0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &bufferInfo);
            descriptorSetManager.fillDescriptotSetsWriteImage(i, 1, 0, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &imageInfo);
            descriptorSetManager.updateDescriptorSets(rbDevice);
        }
    }

    void RBDescriptors::createTextureImage()
    {
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        if (!pixels) {
            throw std::runtime_error("failed to load texture image!");
        }

        RBBufferManager stageBufferManager(rbDevice);
        stageBufferManager.CreateBufferAllocBindMemory(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        void* data = nullptr;
        stageBufferManager.copyMemory(imageSize, data, pixels);

        stbi_image_free(pixels);

        rbImageManager.fillImageInfo(texWidth, texHeight, mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
        rbImageManager.createImage(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, rbImageManager.textureImage, rbImageManager.textureImageMemory);

        VkCommandBuffer commandBuffer = rbCommandBuffer.beginSingleTimeCommands(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
        rbImageManager.transitionImageLayout(commandBuffer, rbImageManager.textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);
        rbCommandBuffer.endSingleTimeCommands(commandBuffer);

        copyBufferToImage(stageBufferManager.buffer, rbImageManager.textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

        VkCommandBuffer commandBufferEnd = rbCommandBuffer.beginSingleTimeCommands(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
        rbImageManager.transitionImageLayout(commandBufferEnd, rbImageManager.textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, mipLevels);
        rbCommandBuffer.endSingleTimeCommands(commandBufferEnd);

        stageBufferManager.destroyBuffer();

        generateMipmaps(rbImageManager.textureImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels);
    }

    void RBDescriptors::createTextureImageView()
    {
        rbImageManager.fillViewInfo(rbImageManager.textureImage, VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels);
        rbImageManager.createImageView();

    }

    void RBDescriptors::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
        VkCommandBuffer commandBuffer = rbCommandBuffer.beginSingleTimeCommands(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

        rbImageManager.copyBufferToImage(commandBuffer, buffer, image, width, height);

        rbCommandBuffer.endSingleTimeCommands(commandBuffer);
    }

    void RBDescriptors::generateMipmaps(VkImage image, VkFormat imageFormat, uint32_t texWidth, uint32_t texHeight, uint32_t mipLevels)
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
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

            if(mipWidth > 1) mipWidth /= 2;
            if(mipHeight > 1) mipHeight /= 2;
        }

        barrier.subresourceRange.baseMipLevel = mipLevels - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        rbCommandBuffer.endSingleTimeCommands(commandBuffer);
    }

    void RBDescriptors::createTextureSampler()
    {
        rbImageManager.fillSampler(VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_COMPARE_OP_LESS_OR_EQUAL, mipLevels);
        rbImageManager.createTextureSampler();
    }

    RBDescriptors::RBDescriptors(RBDevice& device, RBCommandBuffer& commandBuffer, RBBuffer<UniformBufferObject> *uniformBuffers) : rbDevice(device),
    rbCommandBuffer(commandBuffer),
    rbBufferPtr(uniformBuffers),
    descriptorSetManager(device),
    rbImageManager(device)
    {
    }

    void RBDescriptors::InitializeDescriptors()
    {
        createTextureImage();
        createTextureImageView();
        createTextureSampler();
        createDescriptorPool();
        createDescriptorSetLayout();
        createDescriptorSets();
    }

    RBDescriptors::~RBDescriptors()
    {
    }
}
