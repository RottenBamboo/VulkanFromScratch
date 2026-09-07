//
// Created by rottenbamboo on 2026/9/4.
//
#pragma once
#include "RBObject.h"
#include <string>
#include "uuid.h"

namespace RottenBamboo
{
    class RBPrefab
    {
    private:
        RBPrefab();
        ~RBPrefab();
    public:
        RBObjectData prefabData;
    };
}