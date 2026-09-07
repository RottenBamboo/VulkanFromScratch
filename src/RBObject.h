//
// Created by rottenbamboo on 2026/9/4.
//
#pragma once
#include "RBTransform.h"
#include <string>
#include "uuid.h"

namespace RottenBamboo
{
    struct RBObjectData
    {
        Transform tranform;
        std::string name;
        uuids::uuid guid;
    };

    class RBObject
    {
    private:
        RBObject();
        ~RBObject();
    public:
        RBObjectData objectData;
    };
}