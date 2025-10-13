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

        void Clear();

    private:
        RBDevice& device;
        RBCommandBuffer& commandBuffer;
        std::unordered_map<std::string, std::shared_ptr<RBModel>> models;
        std::unordered_map<std::type_index, std::unordered_map<std::string, std::shared_ptr<RBResource>>> resources;
    };
}