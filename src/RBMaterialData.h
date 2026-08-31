//
// Created by rottenbamboo on 2026/8/10.
//
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
        uuids::uuid shaderDefinitionGuid;
        RBShaderDefinition shaderDefinition;
        RBShaderReflection shaderReflection;
    };
}