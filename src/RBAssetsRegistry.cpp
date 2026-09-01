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
        if(!m_GuidToPath.empty())
        {
            m_GuidToPath.clear();
        }
        if(!m_PathToGuid.empty())
        {
            m_PathToGuid.clear();
        }
    }
}