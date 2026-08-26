//
// Created by rottenbamboo on 2026/8/20.
//
#include "RBAssetsRegistry.h"
namespace RottenBamboo
{
    RBAssetsRegistry::RBAssetsRegistry()
    {
        
    }
    RBAssetsRegistry::~RBAssetsRegistry()
    {
        m_GuidToPath.clear();
        m_PathToGuid.clear();
    }
}