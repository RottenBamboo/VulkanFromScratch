//
// Created by rottenbamboo on 2025/4/12.
//

#pragma once

#include "RBDevice.h"

namespace RottenBamboo {
    class RBPipelineUtils {
    public:
        static std::vector<char> readFile(const std::string &filename);
    };
}