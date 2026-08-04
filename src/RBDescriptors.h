//
// Created by rottenbamboo on 2023/6/15.
//

#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/hash.hpp>
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "RBDevice.h"
#include "RBCommandBuffer.h"
#include "RBBuffer.h"
#include "RBDescriptorSetManager.h"
#include "RBImageManager.h"
#include "RBBufferManager.h"
#include "RBResourceShader.h"

#ifdef __ANDROID__
#include <SDL3/SDL_iostream.h>
#endif
namespace RottenBamboo {
    class RBDescriptors {
    private:

        bool isColorAttachment;

        bool enabledDepthAttachment;

        uint32_t imageCount;

        uint32_t bufferCount;

        //std::array<TexturesInfo, ImageCount> imagesInfo;
public:
        std::vector<TexturesInfo> imagesInfo;
private:
        RBDevice &rbDevice;

        RBCommandBuffer &rbCommandBuffer;

        void createDescriptorSetLayout();

        void createDescriptorPool();

        void createDescriptorSets();

        void createDescriptorSetsFrameBuffer(const TextureParams& attachmentParams, const TextureParams& depthParams);

        void createTextureImageView();

        void createTextureImageView(int index);

        void createTextureImageViewFrameBuffer(const TextureParams& attachmentParams,
                                               const TextureParams& depthParams);

        void generateMipmaps(VkImage image, VkFormat imageFormat, VkImageUsageFlags usage, uint32_t texWidth, uint32_t texHeight, uint32_t mipLevels);

        void copyBufferToImage(VkBuffer buffer, VkImage image, VkImageAspectFlags aspectFlags, uint32_t width, uint32_t height);

        void createTextureImage();
        
        void setTextureImage(int index);

        void setTextureImageView(int index);

        void setTextureSampler(int index);
        
        void SetResourceCount(const RBShaderReflection* resourceShaderVertex, const RBShaderReflection* resourceShaderFragment, bool depthEnabled);

        std::pair<uint32_t, uint32_t> GetResourceCount() const;

        void SetDepthAttachmentEnabled(bool depthEnabled);

        void setTextureImageCount();

#ifdef __ANDROID__
        bool LoadFromMemoryAndroid(const std::string& filePath, bool isHDR, std::vector<uint8_t>& buffer);
#endif
        void createTextureImageFrameBuffer(VkExtent2D framebufferExtent,
                                           const TextureParams& attachmentParams,
                                           const TextureParams& depthParams);

        void createTextureSampler();

        void createTextureSamplerFrameBuffer();

    public:

        RBBuffer<UniformBufferShaderVariables> *rbBufferPtr;

        RBDescriptorSetManager descriptorSetManager;

        RBImageManager rbImageManager;

        RBDescriptors(RBDevice &device, RBCommandBuffer &commandBuffer, RBBuffer<UniformBufferShaderVariables> *uniformBuffersonst, const std::vector<std::string> &texturePaths, bool isColorAttachment);

        RBDescriptors(RBDevice &device, RBCommandBuffer &commandBuffer, RBBuffer<UniformBufferShaderVariables> *uniformBuffersonst, const std::vector<TexturesInfo> &texturesInfo, bool isColorAttachment);

        RBDescriptors(RBDevice &device, RBCommandBuffer &commandBuffer, RBBuffer<UniformBufferShaderVariables> *uniformBuffersonst, bool isColorAttachment);

        RBDescriptors(RBDevice &device, RBCommandBuffer &commandBuffer);

        ~RBDescriptors();
        
        void SetResourcesInfos(RBBuffer<UniformBufferShaderVariables> *uniformBuffers, const std::vector<TexturesInfo> &texturesInfo, bool isColorAttachment);
        
        void checkImagesInfo();
                                   
        void SetBufferParams(TextureParams& attachmentParams, TextureParams& depthParams);

        void InitializeDescriptors(const RBShaderReflection* resourceShaderVertex, 
                                   const RBShaderReflection* resourceShaderFragment, 
                                   bool depthEnabled = false);
        
        void InitializeDescriptorsFrameBuffer(VkExtent2D framebufferExtent, 
                                              const TextureParams& attachmentParams,
                                              const TextureParams& depthParams,
                                              const RBShaderReflection* resourceShaderVertex, 
                                              const RBShaderReflection* resourceShaderFragment, 
                                              bool depthEnabled = false);
                                              
        void updateDescriptorSetsTextureImage(int index);

        ImageResourcePtr getTextureImage(int index);

        void refreshTextureImage(int index, const std::string& newTexturePath);

        void releaseTextureImage(ImageResourcePtr imageResourcePtr);
        
        void updateTextureImagePath(int index, const std::string& newTexturePath);

        void ReleaseAllResource();

        inline bool DepthEnabled() const { return enabledDepthAttachment;}
    };
}
