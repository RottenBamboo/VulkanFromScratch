//
// Created by rottenbamboo on 2023/5/26.
//
#pragma once
#define GLFW_INCLUDE_VULKAN

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "RBCommon.h"
#include "RBDevice.h"
#include "RBWindows.h"
#include "RBDescriptors.h"
#include "RBCommandBuffer.h"

namespace RottenBamboo {

    class RBSwapChain {
    public:
        RBDevice &refDevice;
        RBWindows &refWindow;
        RBCommandBuffer &refCommandBuffer;
        RBDescriptors<1, 1> &refDescriptors;
        VkRenderPass renderPass;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        std::vector<VkFramebuffer> swapChainFrameBuffers;
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        VkSwapchainKHR swapChain;
        VkImageView depthImageView;
    private:

        std::vector<VkImageView> swapChainImageViews;
        VkImage colorImage;
        VkDeviceMemory colorImageMemory;
        VkImageView colorImageView;

        VkImage depthImage;
        VkDeviceMemory depthImageMemory;

        VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        VkFormat findDepthFormat();

        void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image,
                         VkDeviceMemory &imageMemory);

        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

        void createImageView();

        void createDepthResources();

        void createColorResources();

        void createRenderPass();

        void createFrameBuffers();

        void createSyncObjects();

        void cleanupSwapChain();

    public:

        void createFrameBuffers(VkImageView& depthImageViewGBuffer);

        void InitializeSwapChain();

        void InitializeSwapChain(VkImageView& depthImageViewGBuffer);

        void recreateSwapChain(VkImageView& depthImageViewGBuffer);

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice &device, VkSurfaceKHR *surface);

        RBSwapChain(RBDevice &device, RBWindows &window, RBCommandBuffer &commandBuffer, RBDescriptors<1, 1> &descriptors);

        void operator=(const RBSwapChain &) = delete;

        RBSwapChain(const RBSwapChain &) = delete;

        void CreateSwapChain(RBDevice &device, RBWindows &window);

        ~RBSwapChain();
    };
}
