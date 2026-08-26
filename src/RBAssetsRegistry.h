
//
// Created by rottenbamboo on 2026/8/20.
//
#pragma once
#include "uuid.h"
#include "RBMetaFile.h"

namespace RottenBamboo
{
    class RBAssetsRegistry
    {
    public:
        RBAssetsRegistry();
        ~RBAssetsRegistry();
        bool Initialize(const std::string& rootPath, const std::string& cachePath) 
        {            
            if (LoadCache(cachePath)) 
            {
                return true;
            }

            if (!ScanDirectory(rootPath)) 
            {
                return false;
            }

            SaveCache(cachePath);
            return true;
        }

        std::optional<std::string> GetPath(const uuids::uuid& guid) const 
        {
            auto it = m_GuidToPath.find(guid);
            if (it != m_GuidToPath.end()) 
            {
                return it->second;
            }
            return std::nullopt;
        }

        std::optional<uuids::uuid> GetGuid(const std::string& path) const 
        {
            std::string normalizedPath = NormalizePathString(path);
            auto it = m_PathToGuid.find(normalizedPath);
            if (it != m_PathToGuid.end()) 
            {
                return it->second;
            }
            return std::nullopt;
        }

        bool Contains(const uuids::uuid& guid) const 
        {
            return m_GuidToPath.find(guid) != m_GuidToPath.end();
        }

        size_t Size() const { return m_GuidToPath.size(); }

        void Register(const uuids::uuid& guid, const std::string& path) 
        {
            auto it = m_GuidToPath.find(guid);
            if (it == m_GuidToPath.end()) 
            {
                std::string normalizedPath = NormalizePathString(path);
                m_GuidToPath[guid] = normalizedPath;
                m_PathToGuid[normalizedPath] = guid;
                m_IsDirty = true;
            }
        }

        void Unregister(const uuids::uuid& guid) 
        {
            auto it = m_GuidToPath.find(guid);
            if (it != m_GuidToPath.end()) 
            {
                m_PathToGuid.erase(it->second);
                m_GuidToPath.erase(it);
                m_IsDirty = true;
            }
        }
        bool SaveCache(const std::string& cachePath) 
        {
            std::filesystem::path filePath(cachePath);
            auto parentPath = filePath.parent_path();
            if (!parentPath.empty() && !std::filesystem::exists(parentPath)) 
            {
                std::filesystem::create_directories(parentPath);
            }
            
            std::ofstream file(cachePath, std::ios::binary);
            if (!file.is_open()) 
            {
                return false;
            }
            
            uint32_t count = static_cast<uint32_t>(m_GuidToPath.size());
            file.write(reinterpret_cast<const char*>(&count), sizeof(count));
            
            for (const auto& [guid, path] : m_GuidToPath) 
            {
                // write GUID 16 byte
                auto bytes = guid.as_bytes();
                file.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());

                // write path length and path string
                uint32_t pathLen = static_cast<uint32_t>(path.size());
                file.write(reinterpret_cast<const char*>(&pathLen), sizeof(pathLen));
                file.write(path.data(), pathLen);
            }

            m_IsDirty = false;
            return true;
        }

        bool LoadCache(const std::string& cachePath) 
        {
            std::ifstream file(cachePath, std::ios::binary);
            if (!file.is_open()) return false;

            uint32_t count = 0;
            file.read(reinterpret_cast<char*>(&count), sizeof(count));
            if (count == 0) return false;

            std::unordered_map<uuids::uuid, std::string> newGuidToPath;
            std::unordered_map<std::string, uuids::uuid> newPathToGuid;
            newGuidToPath.reserve(count);
            newPathToGuid.reserve(count);

            for (uint32_t i = 0; i < count; ++i) 
            {
                // read GUID
                std::array<unsigned char, 16> bytes;
                file.read(reinterpret_cast<char*>(bytes.data()), bytes.size());
                uuids::uuid guid(bytes);

                // read path
                uint32_t pathLen = 0;
                file.read(reinterpret_cast<char*>(&pathLen), sizeof(pathLen));
                std::string path(pathLen, '\0');
                file.read(path.data(), pathLen);

                newGuidToPath[guid] = path;
                newPathToGuid[path] = guid;
            }

            m_GuidToPath.swap(newGuidToPath);
            m_PathToGuid.swap(newPathToGuid);
            m_IsDirty = false;
            return true;
        }
        
    private:
        std::optional<std::pair<uuids::uuid, std::string>> ParseMetaFile(const std::string& metaPath) 
        {
            std::ifstream file(metaPath);
            if (!file.is_open()) return std::nullopt;

            std::filesystem::path metaPathFs(metaPath);
            if (metaPathFs.has_extension() && metaPathFs.extension() != ".meta") 
            {
                return std::nullopt;
            }

            RBData data{};
            m_MetaFile.Load(metaPath, data);

            return std::make_pair(data.GetGUID(), metaPath);
        }
        
        bool ScanDirectory(const std::string& rootPath) 
        {
            if (!std::filesystem::exists(rootPath)) 
            {
                return false;
            }
        
            std::unordered_map<uuids::uuid, std::string> newGuidToPath;
            std::unordered_map<std::string, uuids::uuid> newPathToGuid;
            newGuidToPath.reserve(10000);
            newPathToGuid.reserve(10000);
            
            for (const auto& entry : std::filesystem::recursive_directory_iterator(rootPath)) 
            {
                if (entry.path().extension() == ".meta") 
                {
                    const std::string path = NormalizePathString(entry.path().string());
                    auto result = ParseMetaFile(path);
                    if (result.has_value()) 
                    {
                        auto& [guid, assetPath] = result.value();
                        std::string normalizedPath = NormalizePathString(std::filesystem::relative(assetPath, GET_RESOURCE_ROOT_DIR).string());
                        newGuidToPath[guid] = normalizedPath;
                        newPathToGuid[normalizedPath] = guid;
                    }
                }
            }
            
            m_GuidToPath.swap(newGuidToPath);
            m_PathToGuid.swap(newPathToGuid);
            return true;
        };

        private:
        std::unordered_map<uuids::uuid, std::string> m_GuidToPath;
        std::unordered_map<std::string, uuids::uuid> m_PathToGuid;
        RBMetaFile m_MetaFile;
        bool m_IsDirty = false;
    };
}