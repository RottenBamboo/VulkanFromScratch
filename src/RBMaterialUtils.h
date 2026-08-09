#pragma once

#include <array>
#include <string>

#include "json.h"
#include "RBResourceUtils.h"
#include "RBShaderDefinition.h"
#include "uuid.h"

namespace RottenBamboo {

    static void generateUUID(std::string& id)
    {
         std::random_device rd;
        auto seed_data = std::array<int, std::mt19937::state_size> {};
        std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
        std::mt19937 generator(seq);
        uuids::uuid_random_generator gen{generator};
        id = uuids::to_string(gen());
    }

    struct MaterialData {
        std::string name = "New Material";
        std::string shaderDefinationName = "New Shader";
        uuids::uuid uuid;
        // std::array<float, 4> baseColor {1.0f, 1.0f, 1.0f, 1.0f};
        // std::array<float, 3> emissiveColor {0.0f, 0.0f, 0.0f};
        // float metallic = 0.0f;
        // float roughness = 1.0f;
        // float emissiveStrength = 0.0f;
        // float alphaCutoff = 0.5f;
        // std::string baseColorTexture;
        // std::string normalTexture;
        // std::string metallicRoughnessTexture;
        // std::string emissiveTexture;
        RBShaderDefinition shaderDefinition;
        RBShaderReflection shaderReflection;
    };

}