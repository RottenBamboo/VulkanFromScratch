//
// Create by rottenbamboo on 2025/11/5.
//
#include "RBResourceShader.h"
#include "RBPipelineUtils.h"

#include <vector>
#include <unordered_map>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace RottenBamboo {
    RBResourceShader::RBResourceShader(const std::string &path) : RBResource(path)
    {

    }

    void RBResourceShader::Reflect(const std::string& path, const RenderStage shaderPipelineStage, const std::vector<uint32_t>& spirv)
    {
        if (spirv.size() < 5 || spirv[0] != 0x07230203u)
            return;
        
        spirv_cross::Compiler compiler(spirv);
        auto entries = compiler.get_entry_points_and_stages();
        if (!entries.empty())
            compiler.set_entry_point(entries[0].name, entries[0].execution_model);
        

        //input attribute
        uint32_t currentOffset = 0;
        uint32_t uniformBufferCount = 0;
        uint32_t storageBufferCount = 0;
        uint32_t combinedImageSamplerCount = 0;
        uint32_t sampledImageCount = 0;
        uint32_t samplerCount = 0;
        uint32_t inputFloatCount = 0;
        uint32_t inputVec2Count = 0;
        uint32_t inputVec3Count = 0;
        uint32_t inputVec4Count = 0;
        uint32_t typeCount = 0;
        std::string variableName;
        // SPIR-V ID
        for (auto id : compiler.get_active_interface_variables())
        {
            const auto& type = compiler.get_type_from_variable(id);
            auto storage = compiler.get_storage_class(id);
            const auto& baseType = compiler.get_type(type.self);
            uint32_t set = 0, binding = 0;


            // descriptor set / binding
            if (compiler.has_decoration(id, spv::DecorationDescriptorSet))
                set = compiler.get_decoration(id, spv::DecorationDescriptorSet);
            if (compiler.has_decoration(id, spv::DecorationBinding))
                binding = compiler.get_decoration(id, spv::DecorationBinding);

            VkDescriptorType descType = VK_DESCRIPTOR_TYPE_MAX_ENUM;
           

            if (storage == spv::StorageClassUniform)
            {
                descType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                uniformBufferCount++;
                typeCount = uniformBufferCount;
                std::cout << "Uniform buffer" << std::endl;
                
                RBLOG_INFO("Uniform buffer");
            }
            else if (storage == spv::StorageClassStorageBuffer)
            {
                descType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                storageBufferCount++;
                typeCount = storageBufferCount;
                std::cout << "Storage buffer" << std::endl;
                RBLOG_INFO("Storage buffer");
            }
            else if (storage == spv::StorageClassUniformConstant)
            {
                if (baseType.basetype == spirv_cross::SPIRType::SampledImage)
                {
                    descType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    variableName = compiler.get_name(id);
                    combinedImageSamplerCount++;
                    typeCount = combinedImageSamplerCount;
                    std::cout << "Combined image sampler" << std::endl;
                    RBLOG_INFO("Combined image sampler");
                }
                else if (baseType.basetype == spirv_cross::SPIRType::Image)
                {
                    descType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                    variableName = compiler.get_name(id);
                    sampledImageCount++;
                    typeCount = sampledImageCount;
                    std::cout << "Sampled image" << std::endl;
                    RBLOG_INFO("Sampled image");
                }
                else if (baseType.basetype == spirv_cross::SPIRType::Sampler)
                {
                    descType = VK_DESCRIPTOR_TYPE_SAMPLER;
                    variableName = compiler.get_name(id);
                    samplerCount++;
                    typeCount = samplerCount;
                    std::cout << "Sampler" << std::endl;
                    RBLOG_INFO("Sampler");
                }
            }

            if (descType != VK_DESCRIPTOR_TYPE_MAX_ENUM)
            {
                auto& setInfo = shaderReflection[shaderPipelineStage].descriptorSets[set];
                setInfo.set = set;

                auto& bindingInfo = setInfo.bindings[binding];
                bindingInfo.binding = binding;
                bindingInfo.type = descType;
                bindingInfo.count = 1;
                bindingInfo.name = variableName;
                std::cout << "Descriptor Set: " << set << ", Binding: " << binding << ", Variable Name: " << variableName << std::endl;
                RBLOG_INFO("Descriptor Set: %d, Binding: %d, Variable Name: %s", set, binding, variableName.c_str());
                setInfo.typeCount[descType] = typeCount;
                typeCount = 0;
                continue;
            }

            if (storage == spv::StorageClassPushConstant)
            {
                VkPushConstantRange range{};
                //range.stageFlags = stage;
                range.offset = 0;
                range.size = compiler.get_declared_struct_size(baseType);

                shaderReflection[shaderPipelineStage].pushConstants.push_back(range);
                std::cout << "constant offset " << range.offset << ", constant size " << range.size << std::endl;
                RBLOG_INFO("constant offset %d, constant size %d", range.offset, range.size);
                continue;
            }

            if (storage == spv::StorageClassInput)
            {
                auto base = baseType.basetype;
                RBInputAttributeLocation inputAttr{};
                uint32_t vecSize = baseType.vecsize;
                uint32_t cols = baseType.columns;   //columns counts, more then 1 means matrix
                uint32_t loc = 0;
                if (compiler.has_decoration(id, spv::DecorationLocation))
                {
                    loc = compiler.get_decoration(id, spv::DecorationLocation);
                    inputAttr.location = loc;
                }

                inputAttr.binding = 0;
                std::string typeName;
                std::string offsetStr;
                if (cols > 1)
                {
                    typeName = "mat" + std::to_string(vecSize) + "x" + std::to_string(cols);
                }
                else
                {
                    switch (vecSize)
                    {
                    case 1:
                        typeName = "float";
                        inputAttr.vecSize = vecSize;
                        offsetStr = "0";
                        inputAttr.offset = currentOffset;
                        currentOffset += 1;
                        break;
                    case 2:
                        typeName = "vec2";
                        inputAttr.vecSize = vecSize;
                        offsetStr = "1";
                        inputAttr.offset = currentOffset;
                        currentOffset += 1;
                        break;
                    case 3:
                        typeName = "vec3";
                        inputAttr.vecSize = vecSize;
                        offsetStr = "2";
                        inputAttr.offset = currentOffset;
                        currentOffset += 1;
                        break;
                    case 4: 
                        typeName = "vec4"; 
                        inputAttr.vecSize = vecSize;
                        inputAttr.offset = currentOffset;
                        currentOffset += 1;
                        break;
                    default: 
                        typeName = "unknown"; 
                        inputAttr.vecSize = vecSize;
                        offsetStr = "0";
                        inputAttr.offset = 0;
                        break;
                    }
                }
                const std::string normalizedPath = NormalizePathString(path);
                shaderReflection[shaderPipelineStage].vertexInputs.push_back(inputAttr);
                shaderReflection[shaderPipelineStage].shaderPath = normalizedPath;
                customShaderReflection[normalizedPath] = &shaderReflection[shaderPipelineStage];
                std::cout << "Location:: Binding: " << binding << ", Type: " << typeName << ", Offset: " << currentOffset << std::endl;
                RBLOG_INFO("Location:: Binding: %d, Type: %s, Offset: %d", binding, typeName.c_str(), currentOffset);
            }
        }
    }
VkFormat RBResourceShader::SpirvImageFormatToVkFormat(spv::ImageFormat format)
{
    switch (format)
    {
    case spv::ImageFormatUnknown:    return VK_FORMAT_UNDEFINED;
    case spv::ImageFormatRgba32f:    return VK_FORMAT_R32G32B32A32_SFLOAT;
    case spv::ImageFormatRgba16f:    return VK_FORMAT_R16G16B16A16_SFLOAT;
    case spv::ImageFormatR32f:       return VK_FORMAT_R32_SFLOAT;
    case spv::ImageFormatRgba8:      return VK_FORMAT_R8G8B8A8_UNORM;
    case spv::ImageFormatRgba8Snorm: return VK_FORMAT_R8G8B8A8_SNORM;
    case spv::ImageFormatRg32f:      return VK_FORMAT_R32G32_SFLOAT;
    case spv::ImageFormatRg16f:      return VK_FORMAT_R16G16_SFLOAT;
    case spv::ImageFormatR11fG11fB10f: return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
    case spv::ImageFormatR16f:       return VK_FORMAT_R16_SFLOAT;
    case spv::ImageFormatRgba16:     return VK_FORMAT_R16G16B16A16_UNORM;
    case spv::ImageFormatRgb10A2:    return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
    case spv::ImageFormatRgba8ui:    return VK_FORMAT_R8G8B8A8_UINT;
    case spv::ImageFormatRgba8i:     return VK_FORMAT_R8G8B8A8_SINT;
    case spv::ImageFormatRgba16ui:   return VK_FORMAT_R16G16B16A16_UINT;
    case spv::ImageFormatRgba16i:    return VK_FORMAT_R16G16B16A16_SINT;
    case spv::ImageFormatRgba32ui:   return VK_FORMAT_R32G32B32A32_UINT;
    case spv::ImageFormatRgba32i:    return VK_FORMAT_R32G32B32A32_SINT;
    case spv::ImageFormatRg32ui:     return VK_FORMAT_R32G32_UINT;
    case spv::ImageFormatRg32i:      return VK_FORMAT_R32G32_SINT;
    case spv::ImageFormatRg16ui:     return VK_FORMAT_R16G16_UINT;
    case spv::ImageFormatRg16i:      return VK_FORMAT_R16G16_SINT;
    case spv::ImageFormatRg8:        return VK_FORMAT_R8G8_UNORM;
    case spv::ImageFormatRg8ui:      return VK_FORMAT_R8G8_UINT;
    case spv::ImageFormatRg8i:       return VK_FORMAT_R8G8_SINT;
    case spv::ImageFormatR32ui:      return VK_FORMAT_R32_UINT;
    case spv::ImageFormatR32i:       return VK_FORMAT_R32_SINT;
    case spv::ImageFormatR16ui:      return VK_FORMAT_R16_UINT;
    case spv::ImageFormatR16i:       return VK_FORMAT_R16_SINT;
    case spv::ImageFormatR8:         return VK_FORMAT_R8_UNORM;
    case spv::ImageFormatR8ui:       return VK_FORMAT_R8_UINT;
    case spv::ImageFormatR8i:        return VK_FORMAT_R8_SINT;
    default:                         return VK_FORMAT_UNDEFINED;
    }
}
    void RBResourceShader::Load(RenderStage stage, const std::string& path)
    {
        auto shaderCode = RBPipelineUtils::readFile(path);
        std::cout << "Loaded shader code from : " << path << std::endl;
        RBLOG_INFO("Loaded shader code from : %s", path.c_str());
        size_t codeSize = shaderCode.size();
        if (codeSize % sizeof(uint32_t) != 0)
        {
            RBLOG_FATAL("Char vector size is not aligned to 4 bytes");
            throw std::runtime_error("Char vector size is not aligned to 4 bytes");
        }
        size_t wordCount = codeSize / sizeof(uint32_t);
        shaderSPIRV[stage] = std::vector<uint32_t>(wordCount);
        std::memcpy(shaderSPIRV[stage].data(), shaderCode.data(), codeSize);
    }

    const std::vector<uint32_t>* RBResourceShader::Get(RenderStage shaderStage) const
    {
        if (shaderSPIRV.find(shaderStage) != shaderSPIRV.end())
        {
            return &shaderSPIRV.at(shaderStage);
        }
        else
        {
            return nullptr;
        }
    }

    const ShaderReflectionMap& RBResourceShader::GetReflection() const
    {
        return shaderReflection;
    }

    ShaderReflectionMap& RBResourceShader::GetReflection()
    {
        return shaderReflection;
    }
    
    const RBShaderReflection* RBResourceShader::GetReflection(RenderStage shaderPipelineStage) const
    {
        auto it = shaderReflection.find(shaderPipelineStage);
        return it != shaderReflection.end() ? &shaderReflection.at(shaderPipelineStage) : nullptr;
    }

    RBShaderReflection* RBResourceShader::GetReflection(RenderStage shaderPipelineStage)
    {
        auto it = shaderReflection.find(shaderPipelineStage);
        return it != shaderReflection.end() ? &shaderReflection[shaderPipelineStage] : nullptr;
    }
    
    
    const CustomShaderReflectionMap& RBResourceShader::GetCustomReflection() const
    {
        return customShaderReflection;
    }

    CustomShaderReflectionMap& RBResourceShader::GetCustomReflection()
    {
        return customShaderReflection;
    }
    
    const RBShaderReflection* RBResourceShader::GetCustomReflection(const std::string path) const
    {
        auto it = customShaderReflection.find(path);
        return it != customShaderReflection.end() ? customShaderReflection.at(path) : nullptr;
    }

    RBShaderReflection* RBResourceShader::GetCustomReflection(const std::string path)
    {
        auto it = customShaderReflection.find(path);
        return it != customShaderReflection.end() ? customShaderReflection[path] : nullptr;
    }
    
}