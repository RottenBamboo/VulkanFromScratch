#pragma once

#include <array>
#include <string>

#include "json.h"
#include "RBResourceUtils.h"

namespace RottenBamboo {

    struct MaterialData {
        std::string name = "New Material";
        std::string shaderPathName = "New Shader";
        std::array<float, 4> baseColor {1.0f, 1.0f, 1.0f, 1.0f};
        std::array<float, 3> emissiveColor {0.0f, 0.0f, 0.0f};
        float metallic = 0.0f;
        float roughness = 1.0f;
        float emissiveStrength = 0.0f;
        float alphaCutoff = 0.5f;
        std::string baseColorTexture;
        std::string normalTexture;
        std::string metallicRoughnessTexture;
        std::string emissiveTexture;
        RBShaderReflection* shaderReflection = nullptr;
    };

}