//
// Created by rottenbamboo on 2025/3/17.
//

#ifndef VULKANFROMSCRATCH_RBIMAGEMANAGER_H
#define VULKANFROMSCRATCH_RBIMAGEMANAGER_H
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/hash.hpp>
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "RBDevice.h"

namespace RottenBamboo {
    class RBImageManager {

    private:
        RBDevice &rbDevice;

    public:
        RBImageManager(RBDevice &device);
        ~RBImageManager();
        void createImageView();
        void fillSampler(VkFilter filter, VkSamplerAddressMode addressMode, VkSamplerMipmapMode mipmapMode, VkCompareOp compareOp, uint32_t mipLevels);
        void createTextureSampler();
        void fillSamplerAddressMode(VkSamplerAddressMode addressMode);
        void fillSamplerFilter(VkFilter filter);
        void fillSamplerAnisotropy(bool enable, float maxAnisotropy);
        void fillSamplerCompare(bool enable, VkCompareOp compareOp);
        void fillSamplerMipmap(VkSamplerMipmapMode mipmapMode, float minLod, float maxLod, float mipLodBias);

        void fillViewInfoSubResourceRange(VkImageAspectFlags aspectFlags, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
        void fillViewInfoComponentMapping(VkComponentSwizzle r, VkComponentSwizzle g, VkComponentSwizzle b, VkComponentSwizzle a);
        void fillViewInfo(VkImage &image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
        void transitionImageLayout(VkCommandBuffer &commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
        void copyBufferToImage(VkCommandBuffer &commandBuffer, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        void fillBufferImageCopy(VkBufferImageCopy &region, uint32_t width, uint32_t height);
        void createImage(VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        void fillImageInfo(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage);
        void fillAllocInfo(VkDeviceSize size, uint32_t memoryTypeIndex);

        VkImage textureImage{};
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView{};
        VkSampler textureSampler{};
        VkSamplerCreateInfo samplerInfo{};
        VkImageViewCreateInfo viewInfo{};

    private:
        void createTextureImage();
        void createTextureImageView();
        void fillImageMemoryBarrier(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);

        VkImageMemoryBarrier barrier{};
        VkBufferImageCopy region{};
        VkImageCreateInfo imageInfo{};
        VkMemoryAllocateInfo allocInfo{};
        };
}


#endif //VULKANFROMSCRATCH_RBIMAGEMANAGER_H
