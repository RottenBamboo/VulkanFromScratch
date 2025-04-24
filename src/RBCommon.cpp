//
// Created by rottenbamboo on 2023/5/27.
//

#include "RBCommon.h"


const std::string MODEL_PATH = "../models/Mech.obj";
const std::string TEXTURE_PATH = "../textures/Mech_BaseMap.jpg";

const std::array<std::string, 3> TEXTURE_PATHS_MECH = {
    "../textures/Mech_BaseMap.jpg",
    "../textures/Mech_NormalMap.jpg",
    "../textures/Mech_NormalAO.jpg"
};

uint32_t mipLevels;

VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
VkExtent2D swapChainExtent;
uint32_t currentFrame = 0;
