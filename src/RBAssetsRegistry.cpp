//
// Created by rottenbamboo on 2026/8/20.
//
#include "RBAssetsRegistry.h"
namespace RottenBamboo
{
    RBAssetsRegistry::RBAssetsRegistry()
    {
        m_PathToGuid.reserve(10000);
        m_GuidToPath.reserve(10000);
    }
    RBAssetsRegistry::~RBAssetsRegistry()
    {
        m_GuidToPath.clear();
        m_PathToGuid.clear();
    }
}