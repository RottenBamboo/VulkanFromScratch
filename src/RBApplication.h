//
// Created by rottenbamboo on 2023/5/16.
//

#pragma once
#ifndef VULKANFROMSCRATCH_RBAPPLICATION_H
#define VULKANFROMSCRATCH_RBAPPLICATION_H

#include "RBWindows.h"
#include "RBDevice.h"
#include <stdexcept>
#include <iostream>
#include <string>

namespace RottenBamboo {
    class RBApplication {

    public:
        static const uint32_t WIDTH = 800;
        static const uint32_t HEIGHT = 600;
        void run();
    private:
        RBWindows windows{WIDTH, HEIGHT, "Vulkan"};
        RBDevice device{&windows};
    };
}

#endif //VULKANFROMSCRATCH_RBAPPLICATION_H
