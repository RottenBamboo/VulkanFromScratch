//
// Created by rottenbamboo on 2025/9/30.
//

#pragma once
#include "RBModel.h"
#include <unordered_map>
#include <typeindex>

namespace RottenBamboo {

    class ResourceManager {
    public:
        ResourceManager(RBDevice& device, RBCommandBuffer& commandBuffer);

        std::shared_ptr<RBModel> LoadModels(const std::string& path);

        std::shared_ptr<RBModel> GetModel(const std::string& path);

        template<typename T>
        bool Load(const std::unordered_map<int, std::string>& paths) 
        {
            
            auto typeId = std::type_index(typeid(T));
            auto& table = resources[typeId];

            for(const auto& pair : paths)
            {
                const std::string& path = pair.second;
                if (table.count(path))
                    continue;
                auto res = std::make_shared<T>(path, device, commandBuffer);
                res->Load(path);
                table[path] = res;
            }
            return true;
        }
        
        template<typename T>
        std::shared_ptr<T> Load(const std::string& path) 
        {
            auto typeId = std::type_index(typeid(T));
            auto& table = resources[typeId];

            if (table.count(path))
                return std::static_pointer_cast<T>(table[path]);

            auto res = std::make_shared<T>(path, device, commandBuffer);
            res->Load(path);
            table[path] = res;
            return res;
        }

        template<typename T>
        std::shared_ptr<T> Get(const std::string& path) 
        {
            auto typeId = std::type_index(typeid(T));
            auto itType = resources.find(typeId);
            if (itType != resources.end()) {
                auto& table = itType->second;
                if (table.count(path))
                    return std::static_pointer_cast<T>(table[path]);
            }
            return nullptr;
        }

        template<typename T>
        std::vector<std::string> GetPaths() const
        {
            std::vector<std::string> paths;
            auto typeId = std::type_index(typeid(T));
            auto itType = resources.find(typeId);
            if (itType == resources.end()) {
                return paths;
            }

            const auto& table = itType->second;
            paths.reserve(table.size());
            for (const auto& pair : table) {
                paths.push_back(pair.first);
            }
            std::sort(paths.begin(), paths.end());
            return paths;
        }

        template<typename T>
        bool IsLoaded(const std::string& path) const
        {
            auto typeId = std::type_index(typeid(T));
            auto itType = resources.find(typeId);
            if (itType == resources.end()) {
                return false;
            }
            const auto& table = itType->second;
            return table.find(path) != table.end();
        }

        template<typename T>
        bool Remove(const std::string& path)
        {
            auto typeId = std::type_index(typeid(T));
            auto itType = resources.find(typeId);
            if (itType == resources.end()) {
                return false;
            }
            auto& table = itType->second;
            return table.erase(path) > 0;
        }

        template<typename T>
        std::shared_ptr<T> Reload(const std::string& path)
        {
            Remove<T>(path);
            return Load<T>(path);
        }

        template<typename T>
        size_t Count() const
        {
            auto typeId = std::type_index(typeid(T));
            auto itType = resources.find(typeId);
            if (itType == resources.end()) {
                return 0;
            }
            return itType->second.size();
        }
        
        void Clear();

    private:
        RBDevice& device;
        RBCommandBuffer& commandBuffer;
        std::unordered_map<std::string, std::shared_ptr<RBModel>> models;
        std::unordered_map<std::type_index, std::unordered_map<std::string, std::shared_ptr<RBResource>>> resources;
    };
}