//
// Created by rottenbamboo on 2026/9/4.
//
#pragma once
#include "RBTransform.h"
#include <string>
#include "uuid.h"
#include "RBData.h"

namespace RottenBamboo
{
    struct RBObjectData : public RBData 
    {
        Transform tranform;
        std::string name;
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