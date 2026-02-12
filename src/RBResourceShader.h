//
// Create by rottenbamboo on 2025/11/5.
//
#pragma once

#include <vector>
#include <unordered_map>
#include "RBResource.h"
#include "RBResourceUtils.h"
namespace RottenBamboo {

    using ShaderReflectionMap = std::unordered_map<ShaderPipelineStage, RBShaderReflection>;

    class RBResourceShader : public RBResource {
    public:
    
        RBResourceShader(const std::string &path);
        ~RBResourceShader() = default;

        void Load(ShaderPipelineStage stage, const std::string& path);

        void Load(const std::string& path) override {};

        void Reflect(ShaderReflectionMap& refl, const ShaderPipelineStage shaderPipelineStage, const std::vector<uint32_t>& spirv);
        
        const std::vector<uint32_t>* Get(ShaderPipelineStage shaderStage) const;

        const ShaderReflectionMap& GetReflection() const;

        ShaderReflectionMap& GetReflection();

        const RBShaderReflection* GetReflection(ShaderPipelineStage shaderPipelineStage) const;

        RBShaderReflection* GetReflection(ShaderPipelineStage shaderPipelineStage);
    private:
        std::unordered_map<ShaderPipelineStage, std::vector<uint32_t>> shaderSPIRV;

        ShaderReflectionMap shaderReflection;
    };
}