//
// Created by rottenbamboo on 2023/5/27.
//

#include "RBCommon.h"


const std::string MODEL_PATH = "../models/scene.gltf";
//const std::string TEXTURE_PATH = "../textures/Mech_BaseMap.jpg";
const std::array<std::string, TEXTURE_PATHS_COUNT> TEXTURE_PATH = {
    "../textures/Mech_BaseMap.jpg"
};

const std::array<std::string, TEXTURE_PATHS_MECH_COUNT> TEXTURE_PATHS_MECH = {
    "../textures/Substance_texture_baseColor.jpeg",
    "../textures/Substance_texture_normal.jpeg",
    "../textures/Substance_texture_metallicRoughness.png",
    "../textures/Mech_Emission.jpeg"
};

const std::array<std::string, TEXTURE_PATHS_MECH_GBUFFER_OUTPUT_COUNT> TEXTURE_PATHS_LIGHTING_MECH = {
    "../textures/Mech_BaseMap.jpg",
    "../textures/Mech_Normal.jpg",
    "../textures/Mech_AO.jpeg",
    "../textures/Mech_Emission.jpeg",
    "../textures/Mech_Emission.jpeg"
};

const std::array<std::string, TEXTURE_PATHS_SKYBOX_COUNT> TEXTURE_PATHS_SKYBOX = {
    "../textures/cape_hill_4k.hdr"
};

uint32_t mipLevels;

VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
VkSampleCountFlagBits msaaSamples2 = VK_SAMPLE_COUNT_2_BIT;
VkExtent2D swapChainExtent;
uint32_t currentFrame = 0;

 bool checkbox = false;
 bool isDeviceSupportHDR = false;
 bool isDesiredHDR = true;
