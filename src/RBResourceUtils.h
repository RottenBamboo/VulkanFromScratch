//
// Create by rottenbamboo on 2025/11/5.
//

#pragma once
#include <vulkan/vulkan.h>
namespace RottenBamboo {

    typedef enum ShaderPipelineStage
    {
        SHADER_PIPELINE_STAGE_GBUFFER_FRAGMENT,
        SHADER_PIPELINE_STAGE_GBUFFER_VERTEX,
        SHADER_PIPELINE_STAGE_LIGHTING_FRAGMENT,
        SHADER_PIPELINE_STAGE_LIGHTING_VERTEX,
        SHADER_PIPELINE_STAGE_SKYBOX_FRAGMENT,
        SHADER_PIPELINE_STAGE_SKYBOX_VERTEX,
        SHADER_PIPELINE_STAGE_POST_PROCESSING_FRAGMENT,
        SHADER_PIPELINE_STAGE_POST_PROCESSING_VERTEX
    } ShaderPipelineStage;
    
    struct DescriptorInfo
    {
        VkDescriptorSetLayoutBinding binding;
        VkDescriptorBufferInfo bufferInfo;
        VkDescriptorImageInfo imageInfo;
    };
}