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
        std::string modelGUID;
        std::string modelName;
        std::string materialGUID;
        std::string materialName;

        int childCount;
        std::vector<RBSceneNodeData> children;
        
    };

    struct RBSceneData : public RBData
    {
        std::string name = "Untitled";
        std::vector<RBSceneNodeData> nodes;
    };
}