//
// Created by rottenbamboo on 2025/3/17.
//

#include "RBImageManager.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
//#define TINYOBJLOADER_IMPLEMENTATION
//#include <tiny_obj_loader.h>

namespace RottenBamboo {

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