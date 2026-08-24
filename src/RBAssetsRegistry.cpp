
#include "RBAssetsRegistry.h"
#include <efsw/efsw.hpp>
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