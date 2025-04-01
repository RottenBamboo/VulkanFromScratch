//
// Created by 王旭 on 2025/4/2.
//

#include "LightingPass.h"

namespace RottenBamboo {

    LightingPass::LightingPass(RBDevice &device, RBSwapChain &swapChain, RBDescriptors &descriptors) : RBGraphicPipelineManager(device, swapChain, descriptors) {

    }
    LightingPass::~LightingPass()
    {

    }
}