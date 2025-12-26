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

    void RBResourceShader::Reflect(const std::vector<uint32_t>& spirv)
    {
        if (spirv.size() < 5 || spirv[0] != 0x07230203u)
            return;
        
        spirv_cross::Compiler compiler(spirv);
        auto entries = compiler.get_entry_points_and_stages();
        if (!entries.empty())
            compiler.set_entry_point(entries[0].name, entries[0].execution_model);
        
        // SPIR-V ID
        for (auto id : compiler.get_active_interface_variables())
        {
            if (!compiler.has_decoration(id, spv::DecorationBinding))
                continue;
        
            uint32_t binding =
                compiler.get_decoration(id, spv::DecorationBinding);
        
            uint32_t set = 0;
            if (compiler.has_decoration(id, spv::DecorationDescriptorSet))
                set = compiler.get_decoration(id, spv::DecorationDescriptorSet);
        
            const auto& type = compiler.get_type_from_variable(id);
        
            // descriptor type
            switch (type.storage)
            {
            case spv::StorageClassUniform:
                std::cout << "Uniform buffer  set=" << set
                          << " binding=" << binding << "\n";
                break;
            
            case spv::StorageClassUniformConstant:
                // sampler / sampled image
                std::cout << "Sampled image   set=" << set
                          << " binding=" << binding << "\n";
                break;
            
            case spv::StorageClassStorageBuffer:
                std::cout << "Storage buffer  set=" << set
                          << " binding=" << binding << "\n";
                break;
            
            default:
                break;
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
}