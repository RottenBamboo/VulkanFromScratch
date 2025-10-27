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

        static int SpirvReflectExample(const void* spirv_code, size_t spirv_nbytes);

        static const char* GetShaderStageName(SpvReflectShaderStageFlagBits stage);

        static const char* GetDescriptorTypeName(SpvReflectDescriptorType type);
    };
}