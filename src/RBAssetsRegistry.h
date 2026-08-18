
#include "uuid.h"

namespace RottenBamboo
{
    class RBAssetsRegistry
    {
        struct SceneCache 
        {
            uuids::uuid sceneGuid;
            std::string scenePath;
            std::unordered_map<uuids::uuid, std::string> guidToPath;
            std::vector<uuids::uuid> referencedAssets;
            uint64_t timestamp;  
            bool LoadSceneCache(const uuids::uuid& sceneGuid) 
            {

            }

            void UnloadSceneCache() 
            {

            }

            std::optional<std::string> GetPath(const uuids::uuid& guid) 
            {
                
            }
        };

        std::unordered_map<uuids::uuid, std::string> m_GuidToPath;
        std::unordered_map<std::string, uuids::uuid> m_PathToGuid;
    };
}