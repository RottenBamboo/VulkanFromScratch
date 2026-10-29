//
// Created by rottenbamboo on 2026/9/4.
//

#pragma once
#include "uuid.h"
#include "RBSceneNode.h"
#include "RBResource.h"
#include "RBSceneData.h"

namespace RottenBamboo
{
    class RBScene : RBResource
    {
    public:
        explicit RBScene(const std::string &path, const std::string& nodeName = "Scene", const uuids::uuid& guid = uuids::uuid());

        void SetRoot(std::unique_ptr<RBSceneNode> root);

        RBSceneNode* GetRoot() const;

        void Load(const std::string& loadPath) override;

        bool Save(const std::string& savePath, RBSceneData& materialData);
    private:
        std::unique_ptr<RBSceneNode> m_Root;
        RBSceneData data;
        RBMetaFile metaFile;
    };
}