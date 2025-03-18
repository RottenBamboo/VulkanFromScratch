//
// Created by rottenbamboo on 2025/3/17.
//

#include "RBImageManager.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
//#define TINYOBJLOADER_IMPLEMENTATION
//#include <tiny_obj_loader.h>

namespace RottenBamboo {

    RBImageManager::RBImageManager(RBDevice &device) : rbDevice(device)
    {
        std::cout << "RBImageManager::RBImageManager()" << std::endl;
    }

    RBImageManager::~RBImageManager()
    {
        vkDestroySampler(rbDevice.device, textureSampler, nullptr);
        vkDestroyImageView(rbDevice.device, textureImageView, nullptr);
        std::cout << "RBImageManager::~RBImageManager()" << std::endl;
    }

    void RBImageManager::fillSamplerAddressMode(VkSamplerAddressMode addressMode)
    {
        samplerInfo.addressModeU = addressMode;
        samplerInfo.addressModeV = addressMode;
        samplerInfo.addressModeW = addressMode;
        std::cout << "RBImageManager::fillSamplerAddressMode()" << std::endl;
    }

    void RBImageManager::fillSamplerFilter(VkFilter filter)
    {
        samplerInfo.magFilter = filter;
        samplerInfo.minFilter = filter;
        std::cout << "RBImageManager::fillSamplerFilter()" << std::endl;
    }

    void RBImageManager::fillSamplerAnisotropy(bool enable, float maxAnisotropy)
    {
        samplerInfo.anisotropyEnable = enable;
        if(enable)
        {
            if(maxAnisotropy == 0)
            {
                throw std::runtime_error("Anisotropy is enabled but maxAnisotropy is 0");
            }
            samplerInfo.maxAnisotropy = maxAnisotropy;
        }
        std::cout << "RBImageManager::fillSamplerAnisotropy()" << std::endl;
    }

    void RBImageManager::fillSamplerCompare(bool enable, VkCompareOp compareOp)
    {
        samplerInfo.compareEnable = enable;
        samplerInfo.compareOp = compareOp;
        std::cout << "RBImageManager::fillSamplerCompare()" << std::endl;
    }

    void RBImageManager::fillSamplerMipmap(VkSamplerMipmapMode mipmapMode, float minLod, float maxLod, float mipLodBias)
    {
        samplerInfo.mipmapMode = mipmapMode;
        samplerInfo.minLod = minLod;
        samplerInfo.maxLod = maxLod;
        samplerInfo.mipLodBias = mipLodBias;
        std::cout << "RBImageManager::fillSamplerMipmap()" << std::endl;
    }

    void RBImageManager::fillSampler(VkFilter filter, VkSamplerAddressMode addressMode, VkSamplerMipmapMode mipmapMode, VkCompareOp compareOp, uint32_t mipLevels)
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
    }
    void RBImageManager::createTextureSampler()
    {

        if(vkCreateSampler(rbDevice.device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create texture sampler!");
        }
        std::cout << "RBImageManager::createTextureSampler()" << std::endl;
    }

    void RBImageManager::fillViewInfo(VkImage &image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
    {
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.pNext = nullptr;
        viewInfo.viewType = viewType;
        viewInfo.format = format;
        viewInfo.flags = 0;
        fillViewInfoSubResourceRange(aspectFlags, 0, mipLevels, 0, 1);
        fillViewInfoComponentMapping(VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A);
        std::cout << "RBImageManager::fillViewInfo()" << std::endl;
    }

    void RBImageManager::fillViewInfoComponentMapping(VkComponentSwizzle r, VkComponentSwizzle g, VkComponentSwizzle b, VkComponentSwizzle a)
    {
        viewInfo.components.r = r;
        viewInfo.components.g = g;
        viewInfo.components.b = b;
        viewInfo.components.a = a;
        std::cout << "RBImageManager::fillViewInfoComponentMapping()" << std::endl;
    }
    void RBImageManager::fillViewInfoSubResourceRange(VkImageAspectFlags aspectFlags, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
    {
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = baseMipLevel;
        viewInfo.subresourceRange.levelCount = levelCount;
        viewInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
        viewInfo.subresourceRange.layerCount = layerCount;
        std::cout << "RBImageManager::fillViewInfoSubResourceRange()" << std::endl;
    }

    void RBImageManager::createImageView()
    {
        if(vkCreateImageView(rbDevice.device, &viewInfo, nullptr, &textureImageView) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create texture image view!");
        }
        std::cout << "RBImageManager::createImageView()" << std::endl;
    }

    void RBImageManager::createTextureImage() {

//        int texWidth, texHeight, texChannels;
//        stbi_uc *pixels = stbi_load(TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
//        mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
//        VkDeviceSize imageSize = texWidth * texHeight * 4;
//
//        if (!pixels) {
//            throw std::runtime_error("failed to load texture image!");
//        }
//
//        VkBuffer stagingBuffer;
//        VkDeviceMemory stagingBufferMemory;
//        createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
//
//        void *data;
//        vkMapMemory(rbDevice.device, stagingBufferMemory, 0, imageSize, 0, &data);
//        memcpy(data, pixels, static_cast<size_t>(imageSize));
//        vkUnmapMemory(rbDevice.device, stagingBufferMemory);
//
//        stbi_image_free(pixels);
//
//        createImage(rbDevice, texWidth, texHeight, mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
//                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);
//
//        transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);
//        copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
//        //transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, mipLevels);
//
//        vkDestroyBuffer(rbDevice.device, stagingBuffer, nullptr);
//        vkFreeMemory(rbDevice.device, stagingBufferMemory, nullptr);
//        generateMipmaps(textureImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels);
    }
}