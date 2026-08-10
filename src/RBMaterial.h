//
// Created by rottenbamboo on 2025/10/13.
//

#pragma once
#include "RBDevice.h"
#include "RBResource.h"
#include "RBMaterialData.h"
#include "RBCommandBuffer.h"

namespace RottenBamboo {

    class RBMaterial : public RBResource {
    public:
        //explicit RBMaterial(const std::string& path = "");

        RBMaterial(const std::string &path, RBDevice &device, RBCommandBuffer &commandBuffer);

        void Load(const std::string& path) override;

        bool Save(const std::string& savePath, MaterialData& materialData);

        //bool SaveMeta(const std::string& saveMetaPath) const;

        MaterialData& GetData();
        const MaterialData& GetData() const;

        void Reset();

    private:
        RBDevice &device;
        RBCommandBuffer &commandBuffer;
        MaterialData data;
    };

} // Rottenbamboo

