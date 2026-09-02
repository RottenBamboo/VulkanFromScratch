
//
// Created by rottenbamboo on 2026/8/20.
//
#pragma once
#include "uuid.h"
#include "RBMetaFile.h"
namespace fs = std::filesystem;

namespace RottenBamboo
{
    class RBAssetsRegistry
    {
    public:
        RBAssetsRegistry();
        ~RBAssetsRegistry();
        bool Initialize(const std::string& rootPath, const std::string& cachePath);

        std::optional<std::string> GetPath(const uuids::uuid& guid) const;

        std::optional<uuids::uuid> GetGuid(const std::string& path) const;

        bool Contains(const uuids::uuid& guid) const;

        size_t Size() const;

        void Register(const uuids::uuid& guid, const std::string& path);

        void Unregister(const uuids::uuid& guid);
        bool SaveCache(const std::string& cachePath);

        bool LoadCache(const std::string& cachePath);
        
    private:
        std::optional<std::pair<uuids::uuid, std::string>> ParseMetaFile(const std::string& metaPath);
        
        bool ScanDirectory(const std::string& rootPath);

        private:
        std::unordered_map<uuids::uuid, std::string> m_GuidToPath;
        std::unordered_map<std::string, uuids::uuid> m_PathToGuid;
        RBMetaFile m_MetaFile;
        bool m_IsDirty = false;
    };
}