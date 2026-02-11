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
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace RottenBamboo {
    RBResourceShader::RBResourceShader(const std::string &path) : RBResource(path)
    {

    }

    void RBResourceShader::Reflect(RBShaderReflection& refl, const std::vector<uint32_t>& spirv)
    {
        if (spirv.size() < 5 || spirv[0] != 0x07230203u)
            return;
        
        spirv_cross::Compiler compiler(spirv);
        auto entries = compiler.get_entry_points_and_stages();
        if (!entries.empty())
            compiler.set_entry_point(entries[0].name, entries[0].execution_model);
        

        //input attribute
        uint32_t currentOffset = 0;

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

            if (storage == spv::StorageClassUniform ||
            storage == spv::StorageClassUniformConstant ||
            storage == spv::StorageClassStorageBuffer)
            {
                VkDescriptorType descType = VK_DESCRIPTOR_TYPE_MAX_ENUM;

                if (storage == spv::StorageClassUniform)
                {
                    descType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    std::cout << "Uniform buffer" << std::endl;
                }
                else if (storage == spv::StorageClassStorageBuffer)
                {
                    descType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                    std::cout << "Storage buffer" << std::endl;
                }
                else if (storage == spv::StorageClassUniformConstant)
                {
                    if (baseType.basetype == spirv_cross::SPIRType::SampledImage)
                    {
                        descType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                        std::cout << "Combined image sampler" << std::endl;
                    }
                    else if (baseType.basetype == spirv_cross::SPIRType::Image)
                    {
                        descType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                        std::cout << "Sampled image" << std::endl;
                    }
                    else if (baseType.basetype == spirv_cross::SPIRType::Sampler)
                    {
                        descType = VK_DESCRIPTOR_TYPE_SAMPLER;
                        std::cout << "Sampler" << std::endl;
                    }
                }

                if (descType != VK_DESCRIPTOR_TYPE_MAX_ENUM)
                {
                    auto& setInfo = shaderReflection.descriptorSets[set];
                    setInfo.set = set;

                    auto& bindingInfo = setInfo.bindings[binding];
                    bindingInfo.binding = binding;
                    bindingInfo.type = descType;
                    bindingInfo.count = 1;
                    std::cout << "Descriptor Set: " << set << ", Binding: " << binding << std::endl;
                }

                continue;
            }

            if (storage == spv::StorageClassPushConstant)
            {
                VkPushConstantRange range{};
                //range.stageFlags = stage;
                range.offset = 0;
                range.size = compiler.get_declared_struct_size(baseType);

                shaderReflection.pushConstants.push_back(range);
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
                std::string formatStr;
                std::string offsetStr;
                std::string bingStr;
                if (cols > 1)
                {
                    typeName = "mat" + std::to_string(vecSize) + "x" + std::to_string(cols);
                    formatStr = "MATRIX_FORMAT";
                }
                else
                {
                    switch (vecSize)
                    {
                    case 1:
                        typeName = "float";
                        formatStr = "VK_FORMAT_R32_SFLOAT";
                        inputAttr.format = VK_FORMAT_R32_SFLOAT;
                        offsetStr = "0";
                        inputAttr.offset = currentOffset;
                        currentOffset += 1;
                        break;
                    case 2:
                        typeName = "vec2";
                        formatStr = "VK_FORMAT_R32G32_SFLOAT";
                        inputAttr.format = VK_FORMAT_R32G32_SFLOAT;
                        offsetStr = "1";
                        inputAttr.offset = currentOffset;
                        currentOffset += 1;
                        break;
                    case 3:
                        typeName = "vec3";
                        formatStr = "VK_FORMAT_R32G32B32_SFLOAT";
                        inputAttr.format = VK_FORMAT_R32G32B32_SFLOAT;
                        offsetStr = "2";
                        inputAttr.offset = currentOffset;
                        currentOffset += 1;
                        break;
                    case 4: 
                        typeName = "vec4"; 
                        formatStr = "VK_FORMAT_R32G32B32A32_SFLOAT";
                        inputAttr.format = VK_FORMAT_R32G32B32A32_SFLOAT;
                        inputAttr.offset = currentOffset;
                        currentOffset += 1;
                        break;
                    default: 
                        typeName = "unknown"; 
                        formatStr = "VK_FORMAT_UNDEFINED";
                        inputAttr.format = VK_FORMAT_UNDEFINED;
                        offsetStr = "0";
                        inputAttr.offset = 0;
                        break;
                    }
                }
                shaderReflection.vertexInputs.push_back(inputAttr);
                std::cout << "Location:: Binding: " << binding << ", Type: " << typeName << ", Format: " << formatStr << ", Offset: " << currentOffset << std::endl;
            }
        }
    }

    void RBResourceShader::Load(ShaderPipelineStage stage, const std::string& path)
    {
        auto shaderCode = RBPipelineUtils::readFile(path);
        std::cout << "Loaded shader code from : " << path << std::endl;
        size_t codeSize = shaderCode.size();
        if (codeSize % sizeof(uint32_t) != 0)
        {
            throw std::runtime_error("Char vector size is not aligned to 4 bytes");
        }
        size_t wordCount = codeSize / sizeof(uint32_t);
        shaderSPIRV[stage] = std::vector<uint32_t>(wordCount);
        std::memcpy(shaderSPIRV[stage].data(), shaderCode.data(), codeSize);
    }

    const std::vector<uint32_t>* RBResourceShader::Get(ShaderPipelineStage shaderStage) const
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

    const RBShaderReflection& RBResourceShader::GetReflection() const
    {
        return shaderReflection;
    }

    RBShaderReflection& RBResourceShader::GetReflection()
    {
        return shaderReflection;
    }
    
}