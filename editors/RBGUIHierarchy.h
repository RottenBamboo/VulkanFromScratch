//
// Created by rottenbamboo on 2026/9/11.
//

#pragma once

#include "RBGUIBase.h"
#include "RBShaderDefinition.h"
#include "RBDescriptors.h"
#include "RBSimplePrimitive.h"
#include "RBSceneNode.h"
#include <iostream>
namespace RottenBamboo 
{
    class RBGUIHierarchy : public RBGUIBase 
    {
    public:
        RBGUIHierarchy(RBDevice& device, RBCommandBuffer& commandBuffer);

        virtual ~RBGUIHierarchy() override = default;

        virtual void Initialize(VkRenderPass renderPass) override;

        virtual void Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix) override;

        void SetLayout() override;

        void CreatePrimitive(SimplePrimitiveData* data, std::shared_ptr<RBMaterial> mat, const std::string& modelPath, const std::string& name = "defaultPrimitive", RBSceneNode* parentNode = nullptr);

        void CreateModel(std::shared_ptr<RBMaterial> mat, const std::string& modelPath, const std::string& name = "defaultPrimitive", RBSceneNode* parentNode = nullptr);

        void DeletePrimitive(RBSceneNode* node);

        RBSceneNode* GetSelectedNode();
        RBSceneNode* SetSelectedNode(RBSceneNode* node);

        enum class ClickFrom
        {
            CLICK_FROM_HIERARCHY,
            CLICK_FROM_NODE
        };
        
        private:
        void OnNodeDestroyed(RBSceneNode* node);
        void RenderMenu(VkCommandBuffer& commandBuffer, RBSceneNode* parentNode = nullptr);  
        void PopupMenu(VkCommandBuffer& commandBuffer, ClickFrom clickFrom, RBSceneNode* parentNode = nullptr);  
        void RenderScene(VkCommandBuffer& commandBuffer);  
        RBSceneNode* m_SelectedNode = nullptr;
        RBSceneNode* m_PendingDeleteNode = nullptr;
        RBDevice& rbDevice;
        RBCommandBuffer& rbCommandBuffer;
    };
}