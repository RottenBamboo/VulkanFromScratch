//
// Created by rottenbamboo on 2023/5/16.
//

#pragma once
#ifndef VULKANFROMSCRATCH_RBAPPLICATION_H
#define VULKANFROMSCRATCH_RBAPPLICATION_H

#include "RBWindows.h"
#include "RBDevice.h"
#include "RBCommandBuffer.h"
#include "RBDescriptors.h"
#include "RBSwapChain.h"
#include "RBGraphicPipeline.h"
#include <stdexcept>
#include <iostream>
#include <string>

namespace RottenBamboo {
    class RBApplication {

    public:
        static const uint32_t WIDTH = 800;
        static const uint32_t HEIGHT = 600;
        void run();
        void drawFrame();
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        RBApplication();
    private:
        RBWindows windows{WIDTH, HEIGHT, "Vulkan"};
        RBDevice device{&windows};
        RBCommandBuffer commandBuffer{device};
        RBDescriptors descriptors{device, commandBuffer};
        RBSwapChain swapChain{device, windows, commandBuffer, descriptors};
        RBGraphicPipeline graphicPipeline{swapChain, descriptors};
    };
}

#endif //VULKANFROMSCRATCH_RBAPPLICATION_H
