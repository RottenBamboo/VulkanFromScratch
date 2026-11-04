#include "RBSceneNode.h"
namespace RottenBamboo
{
    RBSceneNode::RBSceneNode(const std::string& name, RBSceneNode* parent, const uuids::uuid& guid, Transform transform)
            : m_Name(name)
            , m_Parent(parent)
            , m_Transform(transform)
            , m_guid(guid)
        {
            m_Resource = nullptr;
            m_Model = nullptr;
            m_Material = nullptr;
        }

    RBSceneNode* RBSceneNode::AddChild(std::unique_ptr<RBSceneNode> child)
    {
        if(!child)
            return nullptr;

        child->SetParent(this);
        m_Children.push_back(std::move(child));
        return m_Children.back().get();
    }

    bool RBSceneNode::RemoveChild(RBSceneNode* child)
    {
        if(!child)
        {
            return false;
        }

        auto it = std::find_if(m_Children.begin(), m_Children.end(), 
        [child](const std::unique_ptr<RBSceneNode>& node) 
        {
            return node.get() == child;
        });

        if (it != m_Children.end())
        {
            m_Children.erase(it);
            return true;
        }

        return false;
    }

    bool RBSceneNode::Release()
    {
        if(m_Model) 
        {
            m_Model.reset();
            m_Resource = nullptr;
        }

        if(m_Material)
        {
            m_Material.reset();
        }
        
        if (m_Parent)
        {
            m_Parent->RemoveChild(this);
        }
        
        for (auto& child : m_Children)
        {
            if (child)
            {
                child->Release();
            }
        }

        m_Children.clear();
        
        return true;
    }

    void RBSceneNode::UpdateWorldTransformRecursive()
    {
    }

    void RBSceneNode::SetDescriptorOffset(int offset)
    {
        m_descriptorBufferOffset = offset;
    }

    int RBSceneNode::GetDescriptorOffset() const
    {
        return m_descriptorBufferOffset;
    }
}