//
// Created by rottenbamboo on 2023/5/27.
//

#include "RBCommon.h"


const std::string MODEL_PATH = "../models/Mech.obj";
//const std::string TEXTURE_PATH = "../textures/Mech_BaseMap.jpg";
const std::array<std::string, TEXTURE_PATHS_COUNT> TEXTURE_PATH = {
    "../textures/Mech_BaseMap.jpg"
};

const std::array<std::string, TEXTURE_PATHS_MECH_COUNT> TEXTURE_PATHS_MECH = {
    "../textures/Mech_BaseMap.jpg",
    "../textures/Mech_Normal.jpg",
    "../textures/Mech_AO.jpeg",
    "../textures/Mech_Emission.jpeg"
};

uint32_t mipLevels;

VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
VkExtent2D swapChainExtent;
uint32_t currentFrame = 0;
