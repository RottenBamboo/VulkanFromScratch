//
// Created by rottenbamboo on 2025/4/2.
//

//#ifndef VULKANFROMSCRATCH_LIGHTINGPASS_H
//#define VULKANFROMSCRATCH_LIGHTINGPASS_H

#pragma once
#include "RBGraphicPipelineManager.h"

namespace RottenBamboo {
    class LightingPass : public RBGraphicPipelineManager {

    protected:
        void setupShaders() override;
    public:
    
        LightingPass(RBDevice &device, RBSwapChain &swapChain, RBDescriptors &descriptors);

        ~LightingPass() override;

        void createGraphicsPipeline() override;

        void InitializeGraphicPipeline() override;
    };
}

//#endif //VULKANFROMSCRATCH_LIGHTINGPASS_H
