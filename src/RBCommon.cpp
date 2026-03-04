//
// Created by rottenbamboo on 2023/5/27.
//

#include "RBCommon.h"

const std::string MODEL_PATH = GET_PROJECT_ROOT_DIR + "models/scene.gltf";

const TexturesInfo fallBackFormat{VK_FORMAT_R8G8B8A8_SRGB, false, ""};

const std::vector<TexturesInfo> inputImagesInfo = {
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, false, GET_PROJECT_ROOT_DIR + "textures/Mech_BaseMap.jpg"}
};

const std::vector<TexturesInfo> inputImageInfoMech = 
{
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, false, GET_PROJECT_ROOT_DIR + "textures/Substance_texture_baseColor.jpeg"},
    TexturesInfo{VK_FORMAT_R8G8B8A8_UNORM, false, GET_PROJECT_ROOT_DIR + "textures/Substance_texture_normal.jpeg"},
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, false, GET_PROJECT_ROOT_DIR + "textures/Substance_texture_metallicRoughness.png"},
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, false, GET_PROJECT_ROOT_DIR + "textures/Mech_Emission.jpeg"}
};

const std::vector<TexturesInfo> inputImageInfoLighting = 
{
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, false, GET_PROJECT_ROOT_DIR + "textures/Mech_BaseMap.jpg"}, // BaseMap
    TexturesInfo{VK_FORMAT_R8G8B8A8_UNORM, false, GET_PROJECT_ROOT_DIR + "textures/Mech_Normal.jpg"}, // Normal
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, false, GET_PROJECT_ROOT_DIR + "textures/Mech_AO.jpeg"}, // AO
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, false, GET_PROJECT_ROOT_DIR + "textures/Mech_Emission.jpeg"},  // Emission
    TexturesInfo{VK_FORMAT_D32_SFLOAT, false, GET_PROJECT_ROOT_DIR + "textures/Mech_Emission.jpeg"}  // Emission
};

const std::vector<ShadersInfo> inputShader = 
{
    ShadersInfo{RottenBamboo::ShaderPipelineStage::SHADER_PIPELINE_STAGE_GBUFFER_FRAGMENT, GET_PROJECT_ROOT_DIR + "shader/bin/gBufferFrag.spv"},
    ShadersInfo{RottenBamboo::ShaderPipelineStage::SHADER_PIPELINE_STAGE_LIGHTING_FRAGMENT, GET_PROJECT_ROOT_DIR + "shader/bin/lightingFrag.spv"},
    ShadersInfo{RottenBamboo::ShaderPipelineStage::SHADER_PIPELINE_STAGE_POST_PROCESSING_FRAGMENT, GET_PROJECT_ROOT_DIR + "shader/bin/skyFrag.spv"},
    ShadersInfo{RottenBamboo::ShaderPipelineStage::SHADER_PIPELINE_STAGE_POST_PROCESSING_VERTEX, GET_PROJECT_ROOT_DIR + "shader/bin/skyVert.spv"},
    ShadersInfo{RottenBamboo::ShaderPipelineStage::SHADER_PIPELINE_STAGE_GBUFFER_VERTEX, GET_PROJECT_ROOT_DIR + "shader/bin/gBufferVert.spv"},
    ShadersInfo{RottenBamboo::ShaderPipelineStage::SHADER_PIPELINE_STAGE_LIGHTING_VERTEX, GET_PROJECT_ROOT_DIR + "shader/bin/lightingVert.spv"}
};


const TextureParams attachmentParams
{
    attachmentUsageFlagBits,
    VK_IMAGE_ASPECT_COLOR_BIT,
    VK_FORMAT_R8G8B8A8_UNORM,
    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
};

const TextureParams depthParams
{
    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
    VK_IMAGE_ASPECT_DEPTH_BIT,
    VK_FORMAT_D32_SFLOAT,
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
};

const std::vector<TexturesInfo> inputImageInfoSkyBox = {
    TexturesInfo{VK_FORMAT_R32G32B32A32_SFLOAT, true, GET_PROJECT_ROOT_DIR + "textures/cape_hill_4k.hdr"}
};

uint32_t mipLevels;

VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
VkSampleCountFlagBits msaaSamples2 = VK_SAMPLE_COUNT_2_BIT;
VkExtent2D swapChainExtent;
uint32_t currentFrame = 0;

bool checkbox = false;
bool isDeviceSupportHDR = false;
bool isDesiredHDR = true;
