//
// Created by rottenbamboo on 2023/6/15.
//

#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/hash.hpp>
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "RBCommon.h"
#include "RBDevice.h"
#include "RBCommandBuffer.h"
#include "RBBuffer.h"
#include "RBDescriptorSetManager.h"
#include "RBImageManager.h"
#include "RBBufferManager.h"

namespace RottenBamboo {
    template<int ImageCount, int BufferCount>
    class RBDescriptors {
    private:

        bool isColorAttachment;
        
        const std::array<std::string, ImageCount> paths;

        RBDevice &rbDevice;

        RBCommandBuffer &rbCommandBuffer;

        void createDescriptorSetLayout();

        void createDescriptorPool();

        void createDescriptorSets();

        void createDescriptorSetsFrameBuffer(std::array<VkImageLayout, TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT> lightingImageLayouts);

        void createTextureImageView();

        void createTextureImageViewFrameBuffer(std::array<VkFormat, ImageCount> imageFormats,
                                               std::array<VkImageUsageFlagBits, ImageCount> imageUsageFlags,
                                               std::array<VkImageAspectFlagBits, ImageCount> imageAspectFlagBits);

        void generateMipmaps(VkImage image, VkFormat imageFormat, uint32_t texWidth, uint32_t texHeight, uint32_t mipLevels);

        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

        void createTextureImage();

        void createTextureImageFrameBuffer(VkExtent2D framebufferExtent, 
                                          std::array<VkFormat, ImageCount> imageFormats,
                                          std::array<VkImageUsageFlagBits, ImageCount> imageUsageFlags);

        void createTextureSampler();

        void createTextureSamplerFrameBuffer();

    public:

        RBBuffer<UniformBufferShaderVariables> *rbBufferPtr;

        RBDescriptorSetManager descriptorSetManager;

        RBImageManager<ImageCount> rbImageManager;

        RBDescriptors(RBDevice &device, RBCommandBuffer &CommandBuffer, RBBuffer<UniformBufferShaderVariables> *uniformBuffersonst, const std::array<std::string, ImageCount> &texturePaths, bool isColorAttachment);

        RBDescriptors(RBDevice &device, RBCommandBuffer &CommandBuffer, RBBuffer<UniformBufferShaderVariables> *uniformBuffersonst, bool isColorAttachment);

        ~RBDescriptors();

        void InitializeDescriptors();
        
        void InitializeDescriptorsFrameBuffer(VkExtent2D framebufferExtent, 
                                              std::array<VkFormat, ImageCount> imageFormats,
                                              std::array<VkImageUsageFlagBits, ImageCount> imageUsageFlags,
                                              std::array<VkImageAspectFlagBits, ImageCount> imageAspectFlagBits,
                                              std::array<VkImageLayout, TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT> lightingImageLayouts);

        void ReleaseAllResource();
    };
}

#include "RBDescriptors.impl.h"
