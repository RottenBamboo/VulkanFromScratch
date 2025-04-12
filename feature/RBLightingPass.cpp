//
// Created by rottenbamboo on 2025/4/2.
//

#include "RBLightingPass.h"

namespace RottenBamboo {


    void LightingPass::setupShaders()
    {
        fillShaderModule("../shader/lightingVert.spv", VK_SHADER_STAGE_VERTEX_BIT, "main");
        fillShaderModule("../shader/lightingFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT, "main");
    }

    LightingPass::LightingPass(RBDevice &device, RBSwapChain &swapChain, RBDescriptors &descriptors) : RBGraphicPipelineManager(device, swapChain, descriptors) 
    {
        //RBGraphicPipelineManager(device, swapChain, descriptors);
        std::cout << "LightingPass::LightingPass()" << std::endl;
    }
    LightingPass::~LightingPass()
    {
        RBGraphicPipelineManager::~RBGraphicPipelineManager();
        std::cout << "LightingPass::~LightingPass()" << std::endl;
    }

    void LightingPass::createGraphicsPipeline()
    {
        RBGraphicPipelineManager::createGraphicsPipeline();
        std::cout << "LightingPass::createGraphicsPipeline()" << std::endl;
    }

    void LightingPass::InitializeGraphicPipeline()
    {
        createGraphicsPipeline();
        std::cout << "RBGraphicPipelineManager::InitializeGraphicPipeline()" << std::endl;
    }
}