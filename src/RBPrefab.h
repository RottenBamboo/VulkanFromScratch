//
// Created by rottenbamboo on 2026/9/4.
//
#pragma once
#include "RBObject.h"
#include <string>
#include "uuid.h"
#include "RBResource.h"
#include "RBMaterialData.h"
#include "RBMetaFile.h"

namespace RottenBamboo
{
    class RBPrefab : public RBResource
    {
    public:
        RBPrefab(const std::string &path, const RBObjectData& data);
        ~RBPrefab();
        virtual void Load(const std::string& path) override;
        bool Save(const std::string& savePath, RBObjectData& data);

    private:
        RBObjectData prefabData;
        RBMetaFile metaFile;
    };
}