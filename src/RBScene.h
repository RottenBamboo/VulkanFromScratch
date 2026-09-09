//
// Created by rottenbamboo on 2026/9/4.
//
#pragma once
#include "uuid.h"
#include "RBSceneNode.h"
#include "RBResource.h"
namespace RottenBamboo
{
    class RBScene
    {
    public:
        explicit RBScene(const std::string& name = "Scene", const uuids::uuid& guid = uuids::uuid());

        RBScene();
    private:
        std::unique_ptr<RBSceneNode> m_Root;
    };
}