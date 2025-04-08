//
// Created by rottenbamboo on 2025/4/2.
//

#include "RBLightingPass.h"

namespace RottenBamboo {

    LightingPass::LightingPass(RBDevice &device, RBSwapChain &swapChain, RBDescriptors &descriptors) : RBGraphicPipelineManager(device, swapChain, descriptors) 
    {
        //RBGraphicPipelineManager::RBGraphicPipelineManager(device, swapChain, descriptors);
        std::cout << "LightingPass::LightingPass()" << std::endl;
    }
    LightingPass::~LightingPass()
    {
        RBGraphicPipelineManager::~RBGraphicPipelineManager();
        std::cout << "LightingPass::~LightingPass()" << std::endl;
    }

    void LightingPass::createGraphicsPipeline(const std::string& vertShaderName, VkShaderStageFlagBits vertStage, const char* pVertName,
                                              const std::string& fragShaderName, VkShaderStageFlagBits fragStage, const char* pfragName)
    {
        RBGraphicPipelineManager::createGraphicsPipeline(vertShaderName, vertStage, pVertName, fragShaderName, fragStage, pfragName);
        std::cout << "LightingPass::createGraphicsPipeline()" << std::endl;
    }
}