//
// Created by rottenbamboo on 2025/10/13.
//

#pragma once
#include "RBDevice.h"
#include "RBResource.h"
#include "RBMaterialData.h"
#include "RBCommandBuffer.h"
#include "RBMetaFile.h"

namespace RottenBamboo {

    class RBMaterial : public RBResource {
    public:
        //explicit RBMaterial(const std::string& path = "");

        RBMaterial(const std::string &path, RBDevice &device, RBCommandBuffer &commandBuffer);

        void Load(const std::string& loadPath) override;

        bool Save(const std::string& savePath, RBMaterialData& materialData);

        //bool SaveMeta(const std::string& saveMetaPath) const;

        RBMaterialData& GetData();
        const RBMaterialData& GetData() const;

        void Reset();

    private:
        RBDevice &device;
        RBCommandBuffer &commandBuffer;
        RBMaterialData data;
        RBMetaFile metaFile;
    };

} // Rottenbamboo

