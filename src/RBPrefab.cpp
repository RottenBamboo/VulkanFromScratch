//
// Created by rottenbamboo on 2026/9/4.
//
#include "RBPrefab.h"
#include "uuid.h"
namespace RottenBamboo
{
    RBPrefab::RBPrefab(const std::string& path, const RBObjectData& data)
        : RBResource(path), prefabData(data)
    {
    }

    RBPrefab::~RBPrefab()
    {

    }
     void RBPrefab::Load(const std::string& path)
     {

     }

    bool RBPrefab::Save(const std::string& savePath, RBObjectData& prefabData)
    {
        
        return false;
    }
}