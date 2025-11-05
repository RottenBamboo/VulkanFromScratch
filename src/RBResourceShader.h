//
// Create by rottenbamboo on 2025/11/5.
//
#pragma once

#include "RBResource.h"
#include "RBResourceUtils.h"
namespace RottenBamboo {

    class RBResourceShader : public RBResource {
    public:
        RBResourceShader(const std::string &path) : RBResource(path) {}
        ~RBResourceShader() = default;

        void Load(ShaderStage stage, const std::string& path);

        void Load(const std::string& path) override {};

        const std::vector<uint32_t>* Get(ShaderStage shaderStage) const;
    private:
        std::unordered_map<ShaderStage, std::vector<uint32_t>> shaderSPIRV;
    };
}