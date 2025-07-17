//
// Created by rottenbamboo on 2023/5/27.
//
#pragma once
#define GLFW_INCLUDE_VULKAN
#define MAX_FRAMES_IN_FLIGHT 2
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/hash.hpp>
#include <vulkan/vulkan.h>
#include <array>
#include <vector>
#include <optional>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include "RBWindows.h"

#define GET_PROJECT_ROOT_DIR RottenBamboo::GetProjectRootPath()
struct TexturesInfo
{
    VkFormat format;
    bool isHDR;
    std::string path;
    TexturesInfo() : format(VK_FORMAT_UNDEFINED), isHDR(false), path(""){}
    TexturesInfo(VkFormat f, bool hdr, const std::string& p) : format(f), isHDR(hdr), path(p) {}
};

struct FrameBuffersInfo
{
    VkFormat format;
    bool isHDR;
    FrameBuffersInfo() : format(VK_FORMAT_UNDEFINED), isHDR(false) {}
    FrameBuffersInfo(VkFormat f, bool hdr) : format(f), isHDR(hdr) {}
};

//extern const int MAX_FRAMES_IN_FLIGHT;
static double timeStamp;
static const float C_intervalTime = 1.0f / 120.0f;
static const int gBufferPassDepthAttachmentCount = 1;
static const int gBufferPassColorAttachmentCount = 4;
static const int gBufferPassAttachmentCount = gBufferPassColorAttachmentCount + gBufferPassDepthAttachmentCount;
static const int lightPassColorAttachmentCount = 1;
static const int skyBoxPassColorAttachmentCount = 1;

#define DEPTH_ATTACHMENT_COUNT 1
#define TEXTURE_PATHS_COUNT 1
#define TEXTURE_PATHS_MECH_COUNT 4
#define TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT (int)(4 + DEPTH_ATTACHMENT_COUNT)
#define TEXTURE_PATHS_SKYBOX_COUNT 1

extern const std::string MODEL_PATH;
extern const TexturesInfo fallBackFormat;
extern const std::array<TexturesInfo, TEXTURE_PATHS_COUNT> inputImagesInfo;
extern const std::array<TexturesInfo, TEXTURE_PATHS_MECH_COUNT> inputImageInfoMech;
extern const std::array<TexturesInfo, TEXTURE_PATHS_SKYBOX_COUNT> inputImageInfoSkyBox;
extern const std::array<TexturesInfo, TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT> inputImageInfoLighting;

extern uint32_t mipLevels;

extern VkSampleCountFlagBits msaaSamples;
extern VkSampleCountFlagBits msaaSamples2;
extern VkExtent2D swapChainExtent;
extern uint32_t currentFrame;
extern bool checkbox;
extern bool isDeviceSupportHDR;
extern bool isDesiredHDR;

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
    glm::vec3 normal;
    glm::vec3 tangent;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 5> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 5> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
        
        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, normal);

        attributeDescriptions[4].binding = 0;
        attributeDescriptions[4].location = 4;
        attributeDescriptions[4].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[4].offset = offsetof(Vertex, tangent);

        return attributeDescriptions;
    }

    bool operator==(const Vertex &other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};

namespace std {
    template<>
    struct hash<Vertex> {
        size_t operator()(Vertex const &vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

struct UniformBufferShaderVariables {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
    glm::vec4 screenSize;
    glm::vec3 cameraPos;
public:
    UniformBufferShaderVariables()
    {
        model = glm::mat4();
        view = glm::mat4();
        proj = glm::mat4();
        screenSize = glm::vec4();
        cameraPos = glm::vec3();
    }
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

namespace RottenBamboo 
{
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities{};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    inline std::string EnsureTrailingSlash(const std::string& path) 
    {
        if (!path.empty() && path.back() != '/')
        {
            return path + "/";
        }
        return path;
    }
    
    inline std::string GetProjectRootPath() {
    #ifdef PROJECT_ROOT_DIR
        return EnsureTrailingSlash(PROJECT_ROOT_DIR);
    #else
        const char* fallback = std::getenv("PROJECT_ROOT_FALLBACK");
        return EnsureTrailingSlash(fallback ? std::string(fallback) : "./");
    #endif
    }
    inline SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice& device, VkSurfaceKHR& surface)
    {
        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
        if(formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
        if(presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    inline bool checkFormatSupported(const std::vector<VkSurfaceFormatKHR>& availableFormats, VkFormat formatTheOne)
    {
        for(const auto& availableFormat : availableFormats)
        {
            if (availableFormat.format == formatTheOne)
            {
                return true;
            }
        }
        return false;
    }

    inline VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for(const auto& availableFormat : availableFormats)
        {
            if(isDesiredHDR)
            {
#ifdef _WIN32
                if(availableFormat.format == VK_FORMAT_R16G16B16A16_SFLOAT && availableFormat.colorSpace == VK_COLOR_SPACE_HDR10_ST2084_EXT)
                {
                    isDeviceSupportHDR = true;
                    std::cout << "Device support HDR" << std::endl;
                    std::cout << "availableFormat.format == VK_FORMAT_R16G16B16A16_SFLOAT " << "availableFormat.colorSpace == VK_COLOR_SPACE_HDR10_ST2084_EXT" << std::endl;
                    return availableFormat;
                }
#else
                
                if (availableFormat.format == VK_FORMAT_R16G16B16A16_SFLOAT && availableFormat.colorSpace == VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT) 
                {
                    isDeviceSupportHDR = true;
                    std::cout << "Device support HDR" << std::endl;
                    std::cout << "availableFormat.format == VK_FORMAT_R16G16B16A16_SFLOAT " << "availableFormat.colorSpace == VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT" << std::endl;
                    return availableFormat;
                }
#endif
                else
                {
                    if(availableFormat.format == VK_FORMAT_R8G8B8A8_SRGB && availableFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
                    {
                        isDeviceSupportHDR = false;
                        std::cout << "Device does not support HDR, fallback to sRGB" << std::endl;
                        return availableFormat;
                    }
                }
            }
            else
            {
                if(availableFormat.format == VK_FORMAT_R8G8B8A8_SRGB && availableFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
                {
                    isDeviceSupportHDR = false;
                    std::cout << "Device does not support HDR" << std::endl;
                    return availableFormat;
                }
            }
        }
        return availableFormats[0];
    }

    inline VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for(const auto& availablePresentMode : availablePresentModes)
        {
            if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    inline VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabalities, SDL_Window* window)
    {
        if(capabalities.currentExtent.width != UINT32_MAX)
        {
            return capabalities.currentExtent;
        } else
        {
            int width, height;
            SDL_GetWindowSizeInPixels(window, &width, &height);
            VkExtent2D actualExtent =
                    {
                            static_cast<uint32_t>(width),
                            static_cast<uint32_t>(height)
                    };
            actualExtent.width = std::clamp(actualExtent.width, capabalities.minImageExtent.width, capabalities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabalities.minImageExtent.height, capabalities.maxImageExtent.height);

            return actualExtent;
        }
    }

    static uint32_t findMemoryType(VkPhysicalDevice &physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }
}
