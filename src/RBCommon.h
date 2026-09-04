//
// Created by rottenbamboo on 2023/5/27.
//
#pragma once
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
#include <filesystem>
#include <regex>
#include "RBWindows.h"
#include "RBResourceUtils.h"
#include "RBLogger.h"

enum RootPathType
{
    PROJECT_ROOT,
    RESOURCE_ROOT
};

//namespace fs = std::filesystem;
#define GET_PROJECT_ROOT_DIR RottenBamboo::GetRootPath(PROJECT_ROOT)
#define GET_RESOURCE_ROOT_DIR RottenBamboo::GetRootPath(RESOURCE_ROOT)
struct TexturesInfo
{
    VkFormat format;
    VkImageAspectFlags aspect;
    
    bool isHDR;
    std::string path;
    bool needUpdate = false;
    TexturesInfo() : format(VK_FORMAT_UNDEFINED), aspect(0), isHDR(false), path(""), needUpdate(false) {}
    TexturesInfo(VkFormat f, VkImageAspectFlags a, bool hdr, const std::string& p) : format(f), aspect(a), isHDR(hdr), path(p), needUpdate(false) {}
};

struct ShadersInfo
{
    RottenBamboo::RenderStage stage;
    std::string path;

    RottenBamboo::PipelineStage pipelineStage;
    std::string shaderDefinitionPath;
    ShadersInfo() : path(""), shaderDefinitionPath(""){}
    //ShadersInfo(const std::string& p, const std::string& sd) : path(p), shaderDefinitionPath(sd) {}
    ShadersInfo(RottenBamboo::RenderStage s, const std::string& p, RottenBamboo::PipelineStage ps, const std::string& sd) : stage(s), path(p), pipelineStage(ps), shaderDefinitionPath(sd) {}
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
#define SHADER_PATHS_COUNT 6
#define TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT (int)(4 + DEPTH_ATTACHMENT_COUNT)
#define TEXTURE_PATHS_SKYBOX_COUNT 1

extern const std::string META_EXTENSION;
extern const std::string MAT_EXTENSION;
extern const std::string SHADER_EXTENSION;

extern std::unordered_map<int, std::string> model_paths;
extern const std::string MODEL_PATH;
extern const std::string TERRAIN_PATH;
extern const std::string SAMURI_PATH;
extern const std::string LOW_POLY_TERRAIN_PATH;
extern const TexturesInfo fallBackFormat;
extern const std::string cachePath;
extern const std::string shaderDefinitionFilePath;
extern const std::string materialsFilePath;
extern const std::string modelsFilePath;
extern const std::string texturesFilePath;
extern const std::string shaderFilePath;
extern const std::string defaultWhiteImagePath;
extern const std::string SKY_SHADER_PATH;
extern const std::string LIGHTING_SHADER_PATH;
extern const std::string DEFAULT_SHADER_REFLECTION;
extern const std::vector<TexturesInfo> inputImageInfoMech;
extern const std::vector<TexturesInfo> inputImageInfoSkyBox;
extern const std::vector<TexturesInfo> inputImageInfoLighting;
extern const std::vector<TexturesInfo> mineCraftTex;

extern uint32_t mipLevels;

#define attachmentUsageFlagBits (VkImageUsageFlags)(VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)

extern VkSampleCountFlagBits msaaSamples;
extern VkSampleCountFlagBits msaaSamples2;
extern VkSampleCountFlagBits msaaSamples4;
extern VkExtent2D swapChainExtent;
extern uint32_t currentFrame;
extern bool checkbox;
extern bool isDeviceSupportHDR;
extern bool isDesiredHDR;

struct TextureParams
{
    VkImageUsageFlags usage;
    VkImageAspectFlags aspect;
    VkFormat format;
    VkImageLayout layout;
};

extern const TextureParams attachmentParams;
extern const TextureParams depthParams;

struct Vertex 
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
    glm::vec3 normal;
    glm::vec3 tangent;

    static VkVertexInputBindingDescription getBindingDescription() 
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 5> getAttributeDescriptions() 
    {
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

    bool operator==(const Vertex &other) const 
    {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};

namespace std 
{
    template<>
    struct hash<Vertex> 
    {
        size_t operator()(Vertex const &vertex) const 
        {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

struct UniformBufferShaderVariables 
{
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

struct QueueFamilyIndices 
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() 
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

#ifdef __ANDROID__
#define VK_KHR_ANDROID_SURFACE_EXTENSION_NAME "VK_KHR_android_surface"
#endif

namespace RottenBamboo 
{
    struct SwapChainSupportDetails 
    {
        VkSurfaceCapabilitiesKHR capabilities{};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    
    inline int countFiles(const std::string& dir) 
    {
        int count = 0;
        for (const auto& entry : std::filesystem::directory_iterator(dir)) 
        {
            if (entry.is_regular_file()) 
            {
                ++count;
            }
        }
        return count;
    }
    
    inline std::vector<std::string> GetDirectoryFileNames(const std::string& dir)
    {
        std::vector<std::string> names;
        names.reserve(countFiles(dir));

        for (const auto& entry : std::filesystem::directory_iterator(dir))
        {
            if (entry.is_regular_file())
            {
                names.push_back(entry.path().filename().string());
            }
        }

        return names;
    }

    inline std::string RemoveFileExtension(std::filesystem::path filePath) 
    {
        return filePath.stem().string();
    }

    inline std::string RemoveFileExtension(const std::string& filePath) 
    {
        size_t dotPos = filePath.find_last_of('.');
        if (dotPos == std::string::npos) {
            return filePath;
        }
        return filePath.substr(0, dotPos);
    }

    inline void GetFileExtension(std::filesystem::path filePath, std::string& out) 
    {
        out = filePath.extension().string();
    }

    inline void GetFileExtension(const std::string& filename, std::string& out) 
    {
        size_t dotPos = filename.find_last_of(".");
        if (dotPos != std::string::npos && dotPos < filename.length() - 1) 
        {
            out = filename.substr(dotPos + 1);
        }
        out = "";
    }
    inline bool FindStringValue(const std::string& text, const std::string& key, std::string& out)
    {
        const std::string pattern = "\\\"" + key + "\\\"\\s*:\\s*\\\"([^\\\"]*)\\\"";
        std::regex regexPattern(pattern);
        std::smatch match;
        if (std::regex_search(text, match, regexPattern) && match.size() >= 2)
        {
            out = match[1].str();
            return true;
        }
        return false;
    }

    inline bool FindFloatValue(const std::string& text, const std::string& key, float& out)
    {
        const std::string pattern = "\\\"" + key + "\\\"\\s*:\\s*([-+]?[0-9]*\\.?[0-9]+(?:[eE][-+]?[0-9]+)?)";
        std::regex regexPattern(pattern);
        std::smatch match;
        if (std::regex_search(text, match, regexPattern) && match.size() >= 2)
        {
            out = std::stof(match[1].str());
            return true;
        }
        return false;
    }

    template <size_t N>
    inline bool FindArrayValue(const std::string& text, const std::string& key, std::array<float, N>& out)
    {
        const std::string pattern = "\\\"" + key + "\\\"\\s*:\\s*\\[(.*?)\\]";
        std::regex regexPattern(pattern);
        std::smatch match;
        if (!std::regex_search(text, match, regexPattern) || match.size() < 2)
        {
            return false;
        }

        std::stringstream ss(match[1].str());
        std::string token;
        size_t index = 0;
        while (std::getline(ss, token, ',') && index < N)
        {
            out[index++] = std::stof(token);
        }
        return index == N;
    }

    static std::string NormalizePathString(std::string path)
    {
        std::replace(path.begin(), path.end(), '\\', '/');
        return path;
    }
    
    static void RemoveExtension(std::string& path)
    {
        size_t pos = path.find_last_of('.');
        if (pos != std::string::npos) 
        {
            path = path.substr(0, pos);
        }
    }

    inline std::string EnsureTrailingSlash(const std::string& path) 
    {
        if (!path.empty() && path.back() != '/')
        {
            return path + "/";
        }
        return path;
    }
    
    inline void printCurrentWorkingDirectory()
    {
        std::filesystem::path cwd = std::filesystem::current_path();
        std::cout << "Current working directory: " << cwd.string() << std::endl;
    }
    
    inline std::string ToLowerCopy(std::string value)
    {
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) 
        {
            return static_cast<char>(std::tolower(c));
        });
        return value;
    }

    inline bool ContainsCaseInsensitive(const std::string& value, const std::string& query)
    {
        if (query.empty()) 
        {
            return true;
        }
        return ToLowerCopy(value).find(ToLowerCopy(query)) != std::string::npos;
    }

    inline bool IsMaterialFile(const std::filesystem::path& path)
    {
       const std::string ext = ToLowerCopy(path.extension().string());
       return ext == ".mat" || ext == ".material";
    }

    inline std::string GetRootPath(RootPathType type) 
    {
#ifdef __ANDROID__    
    return "";  // Android return empty
#else

#if defined(PROJECT_ROOT_DIR) || defined(RESOURCE_ROOT_DIR)
    if (type == PROJECT_ROOT) 
    {
        return EnsureTrailingSlash(PROJECT_ROOT_DIR);
    }
    else if (type == RESOURCE_ROOT)
    {
        return EnsureTrailingSlash(RESOURCE_ROOT_DIR);
    }
    else
    {
        const char* fallback = std::getenv("PROJECT_ROOT_FALLBACK");
        return EnsureTrailingSlash(fallback ? std::string(fallback) : "./");
    }
#endif
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

#if __ANDROID__
            return VK_PRESENT_MODE_FIFO_KHR;
#endif

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

    static uint32_t findMemoryType(VkPhysicalDevice &physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) 
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
            {
                return i;
            }
        }

        RBLOG_FATAL("failed to find suitable memory type!");
        throw std::runtime_error("failed to find suitable memory type!");
    }
}
