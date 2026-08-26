//
// Created by rottenbamboo on 2026/8/18.
//
#pragma once

#include "RBFileSystem.h"
#include "RBDevice.h"
#include "RBResource.h"
#include "RBCommandBuffer.h"
namespace RottenBamboo
{
    class RBMetaFile : public RBFileSystem
    {
        public:
        bool Load(const std::string& loadPath, RBData& data) override;
        bool Save(const std::string& savePath, RBData& data) override;
        ~RBMetaFile() override;
        RBMetaFile();
    };
}