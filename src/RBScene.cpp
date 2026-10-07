//
// Created by rottenbamboo on 2026/9/4.
//
#include "RBScene.h"
namespace RottenBamboo
{
    RBScene::RBScene(const std::string& name, const uuids::uuid& guid) 
     : m_Root(std::make_unique<RBSceneNode>(name, nullptr, guid)) 
    {

    }

    RBScene::RBScene()
    {

    }

    void RBScene::SetRoot(std::unique_ptr<RBSceneNode> root)
    {
        m_Root = std::move(root);
    }

    RBSceneNode* RBScene::GetRoot() const
    {
        return m_Root.get();
    }
}