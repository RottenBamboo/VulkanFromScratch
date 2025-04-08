//
// Created by rottenbamboo on 2025/4/2.
//

//#ifndef VULKANFROMSCRATCH_LIGHTINGPASS_H
//#define VULKANFROMSCRATCH_LIGHTINGPASS_H

#pragma once
//#include "../src/li"
#include "RBGraphicPipelineManager.h"

namespace RottenBamboo {
    class LightingPass : public RBGraphicPipelineManager {

    public:
    
        LightingPass(RBDevice &device, RBSwapChain &swapChain, RBDescriptors &descriptors);

        ~LightingPass() override;

        void createGraphicsPipeline(const std::string& vertShaderName, VkShaderStageFlagBits vertStage, const char* pVertName,
                                    const std::string& fragShaderName, VkShaderStageFlagBits fragStage, const char* pfragName) override;

        void InitializeGraphicPipeline() override;
    };
}

//#endif //VULKANFROMSCRATCH_LIGHTINGPASS_H
