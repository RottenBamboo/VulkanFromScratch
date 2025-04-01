//
// Created by rottenbamboo on 2025/4/2.
//

#ifndef VULKANFROMSCRATCH_LIGHTINGPASS_H
#define VULKANFROMSCRATCH_LIGHTINGPASS_H

//#include "../src/li"
#include "RBGraphicPipelineManager.h"

namespace RottenBamboo {
    class LightingPass : public RBGraphicPipelineManager {

        LightingPass(RBDevice &device, RBSwapChain &swapChain, RBDescriptors &descriptors);

        ~LightingPass();
    };
}

#endif //VULKANFROMSCRATCH_LIGHTINGPASS_H
