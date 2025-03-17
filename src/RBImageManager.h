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
        void createImageView(VkImage &image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
        void createTextureSampler();

        VkImage textureImage;
        VkImageView textureImageView;
        VkSampler textureSampler;
    private:
        void createTextureImage();
        void createTextureImageView();

        VkDeviceMemory textureImageMemory;
        };
}


#endif //VULKANFROMSCRATCH_RBIMAGEMANAGER_H
