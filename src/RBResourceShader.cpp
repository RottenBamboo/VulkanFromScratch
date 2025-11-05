//
// Create by rottenbamboo on 2025/11/5.
//
#include "RBResourceShader.h"
#include "RBPipelineUtils.h"

namespace RottenBamboo {

    void RBResourceShader::Load(ShaderStage stage, const std::string& path)
    {
        auto shaderCode = RBPipelineUtils::readFile(path);
        std::cout << "Loaded shader code from : " << path << std::endl;
        size_t codeSize = shaderCode.size();
        if (codeSize / sizeof(uint32_t) != 0) 
        {
            throw std::runtime_error("Char vector size is not aligned to 4 bytes");
        }
        shaderSPIRV[stage] = std::vector<uint32_t>(codeSize / sizeof(uint32_t));
        std::memcpy(shaderSPIRV[stage].data(), shaderCode.data(), codeSize);
    }

    const std::vector<uint32_t>* RBResourceShader::Get(ShaderStage shaderStage) const
    {
        if (shaderSPIRV.find(shaderStage) != shaderSPIRV.end()) 
        {
            return &shaderSPIRV.at(shaderStage);
        }
        else
        {
            nullptr;
        }
    }
}