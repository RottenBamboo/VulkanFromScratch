
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