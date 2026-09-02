//
// Create by rottenbamboo on 2025/11/5.
//
#pragma once

#include <vector>
#include <unordered_map>
#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>
#include "RBResource.h"
#include "RBResourceUtils.h"
#include "RBMetaFile.h"
namespace RottenBamboo {

    using ShaderReflectionMap = std::unordered_map<uuids::uuid, RBShaderReflection>;
    using CustomShaderReflectionMap = std::unordered_map<std::string, RBShaderReflection*>;

    class RBResourceShader : public RBResource {
    public:
    
        RBResourceShader(const std::string &path);
        ~RBResourceShader() = default;

        void Load(uuids::uuid guid);

        void Load(const std::string& path) override {};

        void Reflect(const std::string& path, const uuids::uuid guid, const std::vector<uint32_t>& spirv);
        
        VkFormat SpirvImageFormatToVkFormat(spv::ImageFormat format);
        
        const std::vector<uint32_t>* Get(uuids::uuid guid) const;

        const ShaderReflectionMap& GetReflection() const;

        ShaderReflectionMap& GetReflection();

        const RBShaderReflection* GetReflection(uuids::uuid guid) const;

        RBShaderReflection* GetReflection(uuids::uuid guid);
        

        const CustomShaderReflectionMap& GetCustomReflection() const;

        CustomShaderReflectionMap& GetCustomReflection();

        const RBShaderReflection* GetCustomReflection(const std::string path) const;

        RBShaderReflection* GetCustomReflection(const std::string path);
        
    private:

        std::unordered_map<uuids::uuid, std::vector<uint32_t>> shaderSPIRV;

        ShaderReflectionMap shaderReflection;
        CustomShaderReflectionMap customShaderReflection;
        RBMetaFile metaFile;

    };
}