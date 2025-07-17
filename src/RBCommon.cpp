//
// Created by rottenbamboo on 2023/5/27.
//

#include "RBCommon.h"


const std::string MODEL_PATH = "../models/scene.gltf";

const std::array<TexturesInfo, TEXTURE_PATHS_COUNT> fallBackFormat = {
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, false, ""}
};

const std::array<TexturesInfo, TEXTURE_PATHS_COUNT> inputImagesInfo = {
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, false, "../textures/Mech_BaseMap.jpg"}
};

const std::array<TexturesInfo, TEXTURE_PATHS_MECH_COUNT> inputImageInfoMech = 
{
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, false, "../textures/Substance_texture_baseColor.jpeg"},
    TexturesInfo{VK_FORMAT_R8G8B8A8_UNORM, false, "../textures/Substance_texture_normal.jpeg"},
    TexturesInfo{VK_FORMAT_R8G8B8A8_UNORM, false, "../textures/Substance_texture_metallicRoughness.png"},
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, false, "../textures/Mech_Emission.jpeg"}
};

const std::array<TexturesInfo, TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT> inputImageInfoLighting = 
{
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, false, "../textures/Mech_BaseMap.jpg"}, // BaseMap
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, false, "../textures/Mech_Normal.jpg"}, // Normal
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, false, "../textures/Mech_AO.jpeg"}, // AO
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, false, "../textures/Mech_Emission.jpeg"},  // Emission
    TexturesInfo{VK_FORMAT_D32_SFLOAT, false, "../textures/Mech_Emission.jpeg"}  // Emission
};

const std::array<TexturesInfo, TEXTURE_PATHS_SKYBOX_COUNT> inputImageInfoSkyBox = {
    TexturesInfo{VK_FORMAT_R32G32B32A32_SFLOAT, true, "../textures/cape_hill_4k.hdr"}
};

uint32_t mipLevels;

VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
VkSampleCountFlagBits msaaSamples2 = VK_SAMPLE_COUNT_2_BIT;
VkExtent2D swapChainExtent;
uint32_t currentFrame = 0;

 bool checkbox = false;
 bool isDeviceSupportHDR = false;
 bool isDesiredHDR = true;
