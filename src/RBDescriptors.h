//
// Created by rottenbamboo on 2023/6/15.
//

#pragma once
#ifndef VULKANFROMSCRATCH_DESCRIPTORS_H
#define VULKANFROMSCRATCH_DESCRIPTORS_H

#include <glm/gtx/hash.hpp>
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "RBCommon.h"
#include "RBDevice.h"
#include "RBCommandBuffer.h"
#include "RBBuffer.h"
#include "RBDescriptorSetLayoutManager.h"


namespace RottenBamboo {
    class RBDescriptors {
    private:
        //duplicated member in graphicpipeline
        VkDescriptorPool descriptorPool;
        VkDeviceMemory textureImageMemory;
        VkImage textureImage;
        VkImageView textureImageView;
        VkSampler textureSampler;
        RBBuffer<UniformBufferObject> *rbBufferPtr;

        RBDevice &rbDevice;
        RBCommandBuffer &rbCommandBuffer;

        void createDescriptorSetLayout();

        void createDescriptorPool();

        void createDescriptorSets();

        void createTextureImageView();

        void generateMipmaps(VkImage image, VkFormat imageFormat, uint32_t texWidth, uint32_t texHeight, uint32_t mipLevels);

        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

        void endSingleTimeCommands(VkCommandBuffer commandBuffer);

        VkCommandBuffer beginSingleTimeCommands();

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice &physicalDevice);

        void createImage(RBDevice &rbDevice, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image,
                    VkDeviceMemory &imageMemory);

        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);

        void createTextureImage();

        void createTextureSampler();

    public:

        //RBDescriptorSetLayoutManager descriptorSetLayout1;
        //VkDescriptorSetLayout descriptorSetLayout;
        RBDescriptorSetLayoutManager descriptorSetLayoutManager;
        std::vector<VkDescriptorSet> descriptorSets;

        RBDescriptors(RBDevice &device, RBCommandBuffer &CommandBuffer, RBBuffer<UniformBufferObject> *uniformBuffers);

        ~RBDescriptors();

        void InitializeDescriptors();
    };
}

#endif //VULKANFROMSCRATCH_DESCRIPTORS_H
