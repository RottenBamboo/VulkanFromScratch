//
// Created by rottenbamboo on 2025/3/17.
//

#include <stb_image.h>
#include "RBImageManager.h"

namespace RottenBamboo {

    RBImageManager::RBImageManager(RBDevice &device) : rbDevice(device)
    {
        std::cout << "RBImageManager::RBImageManager()" << std::endl;
        RBLOG_INFO("RBImageManager::RBImageManager()");
    }

    RBImageManager::~RBImageManager() 
    {
        ReleaseAllResource();
        std::cout << "RBImageManager::~RBImageManager()" << std::endl;
        RBLOG_INFO("RBImageManager::~RBImageManager()");
    }

    void RBImageManager::releaseTextureImage(int index)
    {
        vkDestroyImageView(rbDevice.device, imageBundles[index].imageView, nullptr);
        //vkDestroySampler(rbDevice.device, imageBundles[index].sampler, nullptr);
        vkDestroyImage(rbDevice.device, imageBundles[index].image, nullptr);
        vkFreeMemory(rbDevice.device, imageBundles[index].imageMemory, nullptr);
        imageBundles[index].imageView = VK_NULL_HANDLE;
        //imageBundles[index].sampler = VK_NULL_HANDLE;
        imageBundles[index].image = VK_NULL_HANDLE;
        imageBundles[index].imageMemory = VK_NULL_HANDLE;
    }

    void RBImageManager::ReleaseAllResource() 
    {
        for (int i = 0; i < imageCount; i++)
        {
            vkDestroyImageView(rbDevice.device, imageBundles[i].imageView, nullptr);
            vkDestroySampler(rbDevice.device, imageBundles[i].sampler, nullptr);
            vkDestroyImage(rbDevice.device, imageBundles[i].image, nullptr);
            vkFreeMemory(rbDevice.device, imageBundles[i].imageMemory, nullptr);
            imageBundles[i].imageView = VK_NULL_HANDLE;
            imageBundles[i].sampler = VK_NULL_HANDLE;
            imageBundles[i].image = VK_NULL_HANDLE;
            imageBundles[i].imageMemory = VK_NULL_HANDLE;
        }
        std::cout << "RBImageManager::ReleaseAllResource()" << std::endl;
        RBLOG_INFO("RBImageManager::ReleaseAllResource()");
    }

    void RBImageManager::fillSamplerAddressMode(VkSamplerAddressMode addressMode) 
    {
        samplerInfo.addressModeU = addressMode;
        samplerInfo.addressModeV = addressMode;
        samplerInfo.addressModeW = addressMode;
        std::cout << "RBImageManager::fillSamplerAddressMode()" << std::endl;
        RBLOG_INFO("RBImageManager::fillSamplerAddressMode()");
    }

    void RBImageManager::fillSamplerFilter(VkFilter filter) 
    {
        samplerInfo.magFilter = filter;
        samplerInfo.minFilter = filter;
        std::cout << "RBImageManager::fillSamplerFilter()" << std::endl;
        RBLOG_INFO("RBImageManager::fillSamplerFilter()");
    }

    void RBImageManager::fillSamplerAnisotropy(bool enable, float maxAnisotropy) 
    {
        samplerInfo.anisotropyEnable = enable;
        if (enable) {
            if (maxAnisotropy == 0) {
                RBLOG_FATAL("Anisotropy is enabled but maxAnisotropy is 0");
                throw std::runtime_error("Anisotropy is enabled but maxAnisotropy is 0");
            }
            samplerInfo.maxAnisotropy = maxAnisotropy;
        }
        std::cout << "RBImageManager::fillSamplerAnisotropy()" << std::endl;
        RBLOG_INFO("RBImageManager::fillSamplerAnisotropy()");
    }

    void RBImageManager::fillSamplerCompare(bool enable, VkCompareOp compareOp) 
    {
        samplerInfo.compareEnable = enable;
        samplerInfo.compareOp = compareOp;
        std::cout << "RBImageManager::fillSamplerCompare()" << std::endl;
        RBLOG_INFO("RBImageManager::fillSamplerCompare()");
    }

    void RBImageManager::fillSamplerMipmap(VkSamplerMipmapMode mipmapMode, float minLod, float maxLod, float mipLodBias)
    {
        samplerInfo.mipmapMode = mipmapMode;
        samplerInfo.minLod = minLod;
        samplerInfo.maxLod = maxLod;
        samplerInfo.mipLodBias = mipLodBias;
        std::cout << "RBImageManager::fillSamplerMipmap()" << std::endl;
        RBLOG_INFO("RBImageManager::fillSamplerMipmap()");
    }

    void RBImageManager::setImageCount(uint32_t imageCount)
    {
        this->imageCount = imageCount;
        imageBundles = std::vector<RBImageBundle>(imageCount);
    }

    uint32_t RBImageManager::getImageCount() const
    {
        return imageCount;
    }

    void RBImageManager::fillSampler(VkFilter filter, VkSamplerAddressMode addressMode, VkSamplerMipmapMode mipmapMode, VkCompareOp compareOp, uint32_t mipLevels, uint32_t imageCount) 
    {
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(rbDevice.physicalDevice, &properties);

        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.pNext = nullptr;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        fillSamplerFilter(filter);
        fillSamplerAddressMode(addressMode);
        fillSamplerAnisotropy(VK_TRUE, properties.limits.maxSamplerAnisotropy);
        fillSamplerCompare(VK_FALSE, compareOp);
        fillSamplerMipmap(mipmapMode, 0.0f, static_cast<float>(mipLevels), 0.0f);
        std::cout << "RBImageManager::fillSampler()" << std::endl;
        RBLOG_INFO("RBImageManager::fillSampler()");
    }

    void RBImageManager::createTextureSampler(VkSampler& sampler)
    {
        if (vkCreateSampler(rbDevice.device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS)
        {
            RBLOG_FATAL("failed to indexed create texture sampler!");
            throw std::runtime_error("failed to create indexed texture sampler!");
        }
    }
    void RBImageManager::createTextureSampler()
    {
        for (int i = 0; i < imageCount; i++)
        {
            if (vkCreateSampler(rbDevice.device, &samplerInfo, nullptr, &imageBundles[i].sampler) != VK_SUCCESS)
            {
                RBLOG_FATAL("failed to create texture sampler!");
                throw std::runtime_error("failed to create texture sampler!");
            }
        }
        std::cout << "RBImageManager::createTextureSampler()" << std::endl;
        RBLOG_INFO("RBImageManager::createTextureSampler()");
    }

    void RBImageManager::fillViewInfo(VkImageViewCreateInfo& viewInfo, VkImage &image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
    {
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.pNext = nullptr;
        viewInfo.viewType = viewType;
        viewInfo.format = format;
        viewInfo.flags = 0;
        fillViewInfoSubResourceRange(viewInfo, aspectFlags, 0, mipLevels, 0, 1);
        fillViewInfoComponentMapping(viewInfo, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);
        std::cout << "RBImageManager::fillViewInfo()" << std::endl;
        RBLOG_INFO("RBImageManager::fillViewInfo()");
    }

    void RBImageManager::fillViewInfoComponentMapping(VkImageViewCreateInfo& viewInfo, VkComponentSwizzle r, VkComponentSwizzle g, VkComponentSwizzle b, VkComponentSwizzle a)
    {
        viewInfo.components.r = r;
        viewInfo.components.g = g;
        viewInfo.components.b = b;
        viewInfo.components.a = a;
        std::cout << "RBImageManager::fillViewInfoComponentMapping()" << std::endl;
        RBLOG_INFO("RBImageManager::fillViewInfoComponentMapping()");
    }

    void RBImageManager::fillViewInfoSubResourceRange(VkImageViewCreateInfo& viewInfo, VkImageAspectFlags aspectFlags, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
    {
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = baseMipLevel;
        viewInfo.subresourceRange.levelCount = levelCount;
        viewInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
        viewInfo.subresourceRange.layerCount = layerCount;
        std::cout << "RBImageManager::fillViewInfoSubResourceRange()" << std::endl;
        RBLOG_INFO("RBImageManager::fillViewInfoSubResourceRange()");
    }

    void RBImageManager::createImageView(VkImageViewCreateInfo &viewInfo, VkImageView &imageView)
    {
        if (vkCreateImageView(rbDevice.device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
        {
            RBLOG_FATAL("failed to create texture image view!");
            throw std::runtime_error("failed to create texture image view!");
        }
        std::cout << "RBImageManager::createImageView()" << std::endl;
        RBLOG_INFO("RBImageManager::createImageView()");
    }

    void RBImageManager::RBImageManager::fillImageMemoryBarrier(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask)
    {
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = aspectMask;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = mipLevels;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        std::cout << "RBImageManager::fillImageMemoryBarrier()" << std::endl;
        RBLOG_INFO("RBImageManager::fillImageMemoryBarrier()");

    }

    void RBImageManager::transitionImageLayout(VkCommandBuffer &commandBuffer, VkImage image, VkFormat format, VkImageAspectFlags aspectMask, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels)
    {
        fillImageMemoryBarrier(image, oldLayout, newLayout, aspectMask);
        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else {
            throw std::invalid_argument("unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
                commandBuffer,
                sourceStage,
                destinationStage,
                0,
                0, nullptr,
                0, nullptr,
                1, &barrier
        );
        std::cout << "RBImageManager::transitionImageLayout()" << std::endl;
        RBLOG_INFO("RBImageManager::transitionImageLayout()");
    }

    void RBImageManager::fillImageInfo(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage)
    {
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = numSamples;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        std::cout << "RBImageManager::fillImageInfo()" << std::endl;
        RBLOG_INFO("RBImageManager::fillImageInfo()");
    }

    void RBImageManager::fillAllocInfo(VkDeviceSize size, uint32_t memoryTypeIndex)
    {
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = size;
        allocInfo.memoryTypeIndex = memoryTypeIndex;
        std::cout << "RBImageManager::fileAllocInfo()" << std::endl;
        RBLOG_INFO("RBImageManager::fileAllocInfo()");
    }

    void RBImageManager::createImage(VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
    {
        if (vkCreateImage(rbDevice.device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
            RBLOG_FATAL("failed to create image!");
            throw std::runtime_error("failed to create image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(rbDevice.device, image, &memRequirements);

        fillAllocInfo(memRequirements.size, findMemoryType(rbDevice.physicalDevice, memRequirements.memoryTypeBits, properties));

        if (vkAllocateMemory(rbDevice.device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
            RBLOG_FATAL("failed to allocate image memory!");
            throw std::runtime_error("failed to allocate image memory!");
        }

        vkBindImageMemory(rbDevice.device, image, imageMemory, 0);
        std::cout << "RBImageManager::createImage()" << std::endl;
        RBLOG_INFO("RBImageManager::createImage()");
    }

    void RBImageManager::fillBufferImageCopy(VkBufferImageCopy &region, VkImageAspectFlags aspectFlags, uint32_t width, uint32_t height)
    {
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = aspectFlags;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {
                width,
                height,
                1
        };
    }

    void RBImageManager::copyBufferToImage(VkCommandBuffer &commandBuffer, VkBuffer buffer, VkImage image, VkImageAspectFlags aspectFlags, uint32_t width, uint32_t height) 
    {
        fillBufferImageCopy(region, aspectFlags, width, height);
        vkCmdCopyBufferToImage(
                commandBuffer,
                buffer,
                image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1,
                &region
        );
        std::cout << "RBImageManager::CopyBufferToImage()" << std::endl;
        RBLOG_INFO("RBImageManager::CopyBufferToImage()");
    }
}