//
// Created by rottenbamboo on 2023/5/22.
//

#pragma once
#ifndef VULKANFROMSCRATCH_RBGRAPHICPIPELINE_H
#define VULKANFROMSCRATCH_RBGRAPHICPIPELINE_H

#include <fstream>
#include <tiny_obj_loader.h>
#include "RBCommon.h"
#include "RBSwapChain.h"
#include <stdexcept>
#include <iostream>

namespace RottenBamboo {
    class RBGraphicPipeline {
    public:

        VkPipelineLayout pipelineLayout;

        VkPipeline graphicsPipeline;

        RBGraphicPipeline(RBSwapChain &swapChain, RBDescriptors &descriptors);

        ~RBGraphicPipeline();

        void createGraphicsPipeline();

    private:
        RBSwapChain &rbSwapChain;
        RBDescriptors &rbDescriptors;

        static std::vector<char> readFile(const std::string &filename);

        VkShaderModule createShaderModule(const std::vector<char> &code);
    };
}

#endif //VULKANFROMSCRATCH_RBGRAPHICPIPELINE_H
