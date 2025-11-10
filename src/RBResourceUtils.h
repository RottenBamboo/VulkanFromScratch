//
// Create by rottenbamboo on 2025/11/5.
//

namespace RottenBamboo {
typedef enum ShaderStage 
{
    SHADER_STAGE_VERTEX,
    SHADER_STAGE_TESSELLATION_CONTROL,
    SHADER_STAGE_TESSELLATION_EVALUATION,
    SHADER_STAGE_GEOMETRY,
    SHADER_STAGE_FRAGMENT,
    SHADER_STAGE_COMPUTE,
    SHADER_STAGE_MESH
} VkShaderStage;

struct DescriptorInfo
{
    VkDescriptorSetLayoutBinding binding;
    VkDescriptorBufferInfo bufferInfo;
    VkDescriptorImageInfo imageInfo;
};
}