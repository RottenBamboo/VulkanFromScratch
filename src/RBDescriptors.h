//
// Created by rottenbamboo on 2023/6/15.
//

#pragma once
#ifndef VULKANFROMSCRATCH_DESCRIPTORS_H
#define VULKANFROMSCRATCH_DESCRIPTORS_H
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
    class RBDescriptors {
    private:

        RBDevice &rbDevice;

        RBBuffer<UniformBufferObject> *rbBufferPtr;

        RBCommandBuffer &rbCommandBuffer;

        void createDescriptorSetLayout();

        void createDescriptorPool();

        void createDescriptorSets();

        void createTextureImageView();

        void generateMipmaps(VkImage image, VkFormat imageFormat, uint32_t texWidth, uint32_t texHeight, uint32_t mipLevels);

        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

        void createTextureImage();

        void createTextureSampler();

    public:

        RBDescriptorSetManager descriptorSetManager;
        
        RBImageManager<1> rbImageManager;

        RBDescriptors(RBDevice &device, RBCommandBuffer &CommandBuffer, RBBuffer<UniformBufferObject> *uniformBuffers);

        ~RBDescriptors();

        void InitializeDescriptors();
    };
}

#endif //VULKANFROMSCRATCH_DESCRIPTORS_H
