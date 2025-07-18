//
// Created by rottenbamboo on 2023/5/24.
//
#pragma once
#define VK_USE_PLATFORM_MACOS_MVK
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <string>
#include <set>
#include <unordered_map>
#include "RBCommon.h"
#include "RBWindows.h"

namespace RottenBamboo {

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);

    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    const std::vector<const char *> criticalExtensions = {"VK_KHR_portability_enumeration"};

    static float GetVersionMajor() {
        return VK_API_VERSION_MAJOR(VK_HEADER_VERSION_COMPLETE);
    }

    static float GetVersionMinor() {
        return VK_API_VERSION_MINOR(VK_HEADER_VERSION_COMPLETE);
    }
    class RBDevice {

    public:
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSurfaceKHR surface;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue presentQueue;

    private:

        VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
        RBWindows& rbWindows;


    public:
        void InitializeDevice();

        RBDevice(RBWindows& window);

        ~RBDevice();

        void createInstance();

        void setupDebugMessenger();

        void createSurface(RBWindows& window);

        void pickPhysicalDevice();

        void createLogicalDevice();

        bool isDeviceSuitable(VkPhysicalDevice device);

        bool checkValidationLayerSupport();

        std::vector<const char *> getRequiredExtensions();

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

        bool checkDeviceExtensionSupport(VkPhysicalDevice device);

        VkSampleCountFlagBits getMaxUsableSampleCount();

    };
}
