//
// Created by rottenbamboo on 2026/9/10.
//
#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>
#include "RBResource.h"
#include "RBTransform.h"
#include "uuid.h"
#include "RBMaterial.h"
#include "RBModel.h"

namespace RottenBamboo
{
    class RBSceneNode
    {
    public:
        explicit RBSceneNode(const std::string& name = "Node", RBSceneNode* parent = nullptr, const uuids::uuid& guid = uuids::uuid(), Transform transform = Transform());

        const std::string& GetName() const {return m_Name;}
        void SetName(const std::string& name) { m_Name = name; }

        void SetParent(RBSceneNode* parent) { m_Parent = parent; }
        RBSceneNode* GetParent() const { return m_Parent; }

        RBSceneNode* AddChild(std::unique_ptr<RBSceneNode> child);

        RBSceneNode* CreateChild(const std::string& name = "Node")
        {
            return AddChild(std::make_unique<RBSceneNode>(name));
        }

        RBSceneNode* GetChild(size_t index) { return m_Children[index].get(); }
        const std::vector<std::unique_ptr<RBSceneNode>>& GetChildren() const { return m_Children; }

        size_t GetChildCount() const { return m_Children.size(); }

        void SetTransform(const Transform& transform) { m_Transform = transform; }
        Transform& GetTransform() { return m_Transform; }
        
        void UpdateWorldTransformRecursive();
        uuids::uuid m_guid;
        std::string m_Name;
        RBSceneNode* m_Parent;
        std::vector<std::unique_ptr<RBSceneNode>> m_Children;
        RBResource* m_Resource;
        // convenience pointers for common components/resources
        std::shared_ptr<RBModel> m_Model = nullptr;
        //class RBMaterial* m_Material = nullptr;
        std::shared_ptr<RBMaterial> m_Material = nullptr;
        Transform m_Transform;
    };
}