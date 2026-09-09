#include "RBSceneNode.h"
namespace RottenBamboo
{
    RBSceneNode::RBSceneNode(const std::string& name, RBSceneNode* parent, const uuids::uuid& guid, Transform transform)
            : m_Name(name)
            , m_Parent(parent)
            , m_Transform(transform)
            , m_guid(guid)
        {
        }
    RBSceneNode* RBSceneNode::AddChild(std::unique_ptr<RBSceneNode> child)
    {
        if(!child)
            return nullptr;

        child->SetParent(this);
        m_Children.push_back(std::move(child));
        return m_Children.back().get();
    }

    void RBSceneNode::UpdateWorldTransformRecursive()
    {
    }
}