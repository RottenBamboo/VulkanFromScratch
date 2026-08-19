//
// Created by rottenbamboo on 2023/5/27.
//

#include "RBCommon.h"

std::unordered_map<int, std::string> model_paths;

const std::string META_EXTENSION = ".meta";
const std::string MAT_EXTENSION = ".mat";
const std::string SHADER_EXTENSION = ".shader";

const std::string MODEL_PATH = GET_PROJECT_ROOT_DIR + "models/mach.gltf";

const std::string TERRAIN_PATH = GET_PROJECT_ROOT_DIR + "models/CraterCityMesh.fbx";

const std::string SAMURI_PATH = GET_PROJECT_ROOT_DIR + "models/samuri.gltf";

const std::string LOW_POLY_TERRAIN_PATH = GET_PROJECT_ROOT_DIR + "models/low_poly_terrain.gltf";

const TexturesInfo fallBackFormat{VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, false, ""};

const std::string shaderDefinitionFilePath = "shaderDefinition/";
const std::string materialsFilePath = "materials/";
const std::string modelsFilePath = GET_PROJECT_ROOT_DIR + "models/";
const std::string texturesFilePath = GET_PROJECT_ROOT_DIR + "textures/";
const std::string shaderFilePath = GET_PROJECT_ROOT_DIR + "shader/";
const std::string defaultWhiteImagePath = "textures/default_white.jpeg";

const std::vector<TexturesInfo> inputImagesInfo = {
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/Mech_BaseMap.jpg"}
};

const std::vector<TexturesInfo> lowPolyTerrainTex = 
{
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/low_poly_terrain_basecolor.jpeg"},
    TexturesInfo{VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/low_poly_terrain_normal.jpeg"},
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/low_poly_terrain_metal_roughness.png"},
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/Mech_Emission.jpeg"}
};

const std::vector<TexturesInfo> samuriTex = 
{
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/samuri_diffuse.jpeg"},
    TexturesInfo{VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/samuri_normal.jpeg"},
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/samuri_specular_glossiness.png"},
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/Mech_Emission.jpeg"}
};

const std::vector<TexturesInfo> inputImageInfoMech = 
{
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/Substance_texture_baseColor.jpeg"},
    TexturesInfo{VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/Substance_texture_normal.jpeg"},
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/Substance_texture_metallicRoughness.png"},
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/Mech_Emission.jpeg"}
};

const std::vector<TexturesInfo> inputImageInfoLighting = 
{
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/Mech_BaseMap.jpg"}, // BaseMap
    TexturesInfo{VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/Mech_Normal.jpg"}, // Normal
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/Mech_AO.jpeg"}, // AO
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/Mech_Emission.jpeg"},  // Emission
    TexturesInfo{VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT, false, "textures/Mech_Emission.jpeg"}  // Emission
};

const std::vector<TexturesInfo> inputImageTerrain = 
{
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/AlienPlanet1_Diffuse.jpg"}, // BaseMap
    TexturesInfo{VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/AlienPlanet1_FinalBump1.png"}, // Normal
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/AlienPlanet1_Wetness.png"}, // AO
    TexturesInfo{VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, false, "textures/Mech_Emission.jpeg"},  // Emission
    TexturesInfo{VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT, false, "textures/Mech_Emission.jpeg"}  // Emission
};

const std::vector<ShadersInfo> inputShader = 
{
    ShadersInfo{RottenBamboo::RenderStage::RENDER_STAGE_GBUFFER_FRAGMENT, GET_PROJECT_ROOT_DIR + "shader/bin/gBufferFrag.spv", RottenBamboo::PipelineStage::PIPELINE_STAGE_FRAGMENT, "shaderDefinition/gBufferDefinition.shader"},
    ShadersInfo{RottenBamboo::RenderStage::RENDER_STAGE_LIGHTING_FRAGMENT, GET_PROJECT_ROOT_DIR + "shader/bin/lightingFrag.spv", RottenBamboo::PipelineStage::PIPELINE_STAGE_FRAGMENT, "shaderDefinition/lightingDefinition.shader"},
    ShadersInfo{RottenBamboo::RenderStage::RENDER_STAGE_POST_PROCESSING_FRAGMENT, GET_PROJECT_ROOT_DIR + "shader/bin/skyFrag.spv", RottenBamboo::PipelineStage::PIPELINE_STAGE_FRAGMENT, "shaderDefinition/skyDefinition.shader"},
    ShadersInfo{RottenBamboo::RenderStage::RENDER_STAGE_POST_PROCESSING_VERTEX, GET_PROJECT_ROOT_DIR + "shader/bin/skyVert.spv", RottenBamboo::PipelineStage::PIPELINE_STAGE_VERTEX, "shaderDefinition/skyDefinition.shader"},
    ShadersInfo{RottenBamboo::RenderStage::RENDER_STAGE_GBUFFER_VERTEX, GET_PROJECT_ROOT_DIR + "shader/bin/gBufferVert.spv", RottenBamboo::PipelineStage::PIPELINE_STAGE_VERTEX, "shaderDefinition/gBufferDefinition.shader"},
    ShadersInfo{RottenBamboo::RenderStage::RENDER_STAGE_LIGHTING_VERTEX, GET_PROJECT_ROOT_DIR + "shader/bin/lightingVert.spv", RottenBamboo::PipelineStage::PIPELINE_STAGE_VERTEX, "shaderDefinition/lightingDefinition.shader"}
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
    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
    VK_IMAGE_ASPECT_DEPTH_BIT,
    VK_FORMAT_D32_SFLOAT,
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
};

const std::vector<TexturesInfo> inputImageInfoSkyBox = {
    TexturesInfo{VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_ASPECT_COLOR_BIT, true, "textures/cape_hill_4k.hdr"}
};

uint32_t mipLevels;

VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
VkSampleCountFlagBits msaaSamples2 = VK_SAMPLE_COUNT_2_BIT;
VkSampleCountFlagBits msaaSamples4 = VK_SAMPLE_COUNT_4_BIT;
VkExtent2D swapChainExtent;
uint32_t currentFrame = 0;

bool checkbox = false;
bool isDeviceSupportHDR = false;
bool isDesiredHDR = true;
