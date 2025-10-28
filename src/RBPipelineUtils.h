//
// Created by rottenbamboo on 2025/4/12.
//

#pragma once

#include "RBDevice.h"
#include "spirv_reflect.h"

namespace RottenBamboo {
    class RBPipelineUtils {
    public:
        static std::vector<char> readFile(const std::string &filename);

        static void ReflectShader(const uint32_t* spirv_code, size_t spirv_nbytes);
    };
}