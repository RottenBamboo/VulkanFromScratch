#pragma once

#include <array>

#include "json.h"
#include "RBResourceUtils.h"
#include "RBShaderDefinition.h"
#include "RBData.h"

namespace RottenBamboo 
{
    struct RBMaterialData : public RBData 
    {
        std::string name = "New Material";
        std::string shaderDefinationName = "New Shader";
        RBShaderDefinition shaderDefinition;
        RBShaderReflection shaderReflection;
    };
}