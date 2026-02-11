//
// Create by rottenbamboo on 2025/11/5.
//
#pragma once

#include <vector>
#include <unordered_map>
#include "RBResource.h"
#include "RBResourceUtils.h"
namespace RottenBamboo {

    class RBResourceShader : public RBResource {
    public:
    
        RBResourceShader(const std::string &path);
        ~RBResourceShader() = default;

        void Load(ShaderPipelineStage stage, const std::string& path);

        void Load(const std::string& path) override {};

        void Reflect(RBShaderReflection& refl, const std::vector<uint32_t>& spirv);
        
        const std::vector<uint32_t>* Get(ShaderPipelineStage shaderStage) const;

        const RBShaderReflection& GetReflection() const;

        RBShaderReflection& GetReflection();
    private:
        std::unordered_map<ShaderPipelineStage, std::vector<uint32_t>> shaderSPIRV;

        RBShaderReflection shaderReflection;
    };
}