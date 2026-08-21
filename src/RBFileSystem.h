//
// Create by rottenbamboo on 2026/8/20.
//
#pragma once
#include "uuid.h"
#include "RBData.h"
namespace RottenBamboo 
{
    class RBFileSystem
    {
        public:
        virtual bool Save(const std::string& savePath, RBData& data) = 0;
        virtual bool Load(const std::string& loadPath, RBData& data) = 0;
        virtual ~RBFileSystem();
        RBFileSystem();
    };
}