//
// Created by rottenbamboo on 2025/4/2.
//

#pragma once

#include "RBLightingPass.h"

namespace RottenBamboo {

    LightingPass::LightingPass(RBDevice &device, RBSwapChain &swapChain, RBDescriptors &descriptors) : RBGraphicPipelineManager(device, swapChain, descriptors) {
        //RBGraphicPipelineManager::RBGraphicPipelineManager(device, swapChain, descriptors);
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
}