//
// Created by rottenbamboo on 2023/5/22.
//

#pragma once
#ifndef VULKANFROMSCRATCH_RBGRAPHICPIPELINEMANAGER_H
#define VULKANFROMSCRATCH_RBGRAPHICPIPELINEMANAGER_H

#include <fstream>
#include <tiny_obj_loader.h>
#include "RBCommon.h"
#include "RBSwapChain.h"
#include <stdexcept>
#include <iostream>

namespace RottenBamboo {
    class RBGraphicPipelineManager {
    public:

        VkPipelineLayout pipelineLayout;

        VkPipeline graphicsPipeline;

        RBGraphicPipelineManager(RBSwapChain &swapChain, RBDescriptors &descriptors);

        ~RBGraphicPipelineManager();

        void createGraphicsPipeline();

        void InitializeGraphicPipeline();
    private:
        RBSwapChain &rbSwapChain;
        RBDescriptors &rbDescriptors;

        static std::vector<char> readFile(const std::string &filename);

        VkShaderModule createShaderModule(const std::vector<char> &code);
    };
}

#endif //VULKANFROMSCRATCH_RBGRAPHICPIPELINEMANAGER_H
