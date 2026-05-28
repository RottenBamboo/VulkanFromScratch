//
// Created by rottenbamboo on 2025/10/13.
//

#pragma once
#include "RBDevice.h"
#include "RBResource.h"
#include "RBMaterialUtils.h"
#include "RBCommandBuffer.h"

namespace RottenBamboo {

    class RBMaterial : public RBResource {
    public:
        //explicit RBMaterial(const std::string& path = "");

        RBMaterial(const std::string &path, RBDevice &device, RBCommandBuffer &commandBuffer);

        void Load(const std::string& path) override;

        bool Save(const std::string& path = "", const MaterialData& material = MaterialData{}) const;

        MaterialData& GetData();
        const MaterialData& GetData() const;

        void Reset();

    private:
        RBDevice &device;
        RBCommandBuffer &commandBuffer;
        MaterialData data;
    };

} // Rottenbamboo

