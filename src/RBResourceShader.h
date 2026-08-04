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
namespace RottenBamboo {

    using ShaderReflectionMap = std::unordered_map<RenderStage, RBShaderReflection>;
    using CustomShaderReflectionMap = std::unordered_map<std::string, RBShaderReflection*>;

    class RBResourceShader : public RBResource {
    public:
    
        RBResourceShader(const std::string &path);
        ~RBResourceShader() = default;

        void Load(RenderStage stage, const std::string& path);

        void Load(const std::string& path) override {};

        void Reflect(const std::string& path, const RenderStage shaderPipelineStage, const std::vector<uint32_t>& spirv);
        
        VkFormat SpirvImageFormatToVkFormat(spv::ImageFormat format);
        
        const std::vector<uint32_t>* Get(RenderStage shaderStage) const;

        const ShaderReflectionMap& GetReflection() const;

        ShaderReflectionMap& GetReflection();

        const RBShaderReflection* GetReflection(RenderStage shaderPipelineStage) const;

        RBShaderReflection* GetReflection(RenderStage shaderPipelineStage);
        

        const CustomShaderReflectionMap& GetCustomReflection() const;

        CustomShaderReflectionMap& GetCustomReflection();

        const RBShaderReflection* GetCustomReflection(const std::string path) const;

        RBShaderReflection* GetCustomReflection(const std::string path);
        
    private:

        std::unordered_map<RenderStage, std::vector<uint32_t>> shaderSPIRV;

        ShaderReflectionMap shaderReflection;
        CustomShaderReflectionMap customShaderReflection;
    };
}