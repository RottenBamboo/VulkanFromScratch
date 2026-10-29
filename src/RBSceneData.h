//
// Created by rottenbamboo on 2026/10/29.
//

#pragma once

#include <array>
#include <vector>

#include <glm/glm.hpp>

#include "json.h"
#include "RBResourceUtils.h"
#include "RBShaderDefinition.h"
#include "RBData.h"

namespace RottenBamboo 
{
    struct RBSceneNodeData : public RBData
    {
        std::string name = "SceneNode";
        vector3 position;
        vector3 rotate;
        vector3 scale;
    };

    struct RBSceneData : public RBData
    {
        std::string name = "Scene";
        std::string rootGuid;
        std::vector<RBSceneNodeData> nodes;
    };
}