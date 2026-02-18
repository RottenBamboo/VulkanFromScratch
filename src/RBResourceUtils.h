//
// Create by rottenbamboo on 2025/11/5.
//

#pragma once
#include <vulkan/vulkan.h>
#include <unordered_map>
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
    
    struct RBDescriptorBinding
    {
        uint32_t            binding;
        VkDescriptorType    type;
        uint32_t            count;
    };

    struct RBInputAttributeLocation
    {
        uint32_t            location;
        uint32_t            binding;
        uint32_t            vecSize;
        uint32_t            offset;
    };
    
    struct RBDescriptorSetLayoutInfo
    {
        uint32_t set;
        std::unordered_map<uint32_t, RBDescriptorBinding> bindings;
        std::unordered_map<VkDescriptorType, uint32_t> typeCount;
    };

    struct RBShaderReflection
    {
        std::unordered_map<uint32_t, RBDescriptorSetLayoutInfo> descriptorSets;
        std::vector<VkPushConstantRange> pushConstants;
        std::vector<RBInputAttributeLocation> vertexInputs;

        uint32_t GetUniformBufferCount(uint32_t setIndex) const
        {
            auto it = descriptorSets.find(setIndex);
            if(it != descriptorSets.end())
            {
            auto itFragmentBuffer = it->second.typeCount.find(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
            if(itFragmentBuffer != it->second.typeCount.end())
            {
                return itFragmentBuffer->second;
            }
        }

        return 0;
    }
    };
}