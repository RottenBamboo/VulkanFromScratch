//
// Created by rottenbamboo on 2025/10/13.
//

#pragma once

#include "RBResource.h"
#include "RBMaterialUtils.h"

namespace RottenBamboo {

    class RBMaterial : public RBResource {
    public:
        explicit RBMaterial(const std::string& path = "");

        void Load(const std::string& path) override;

        bool Save(const std::string& path = "") const;

        MaterialData& GetData();
        const MaterialData& GetData() const;

        void Reset();

    private:
        MaterialData data;
    };

} // Rottenbamboo

