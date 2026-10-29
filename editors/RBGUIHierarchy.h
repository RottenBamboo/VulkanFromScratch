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

        void CreatePrimitive(SimplePrimitiveData* data, std::shared_ptr<RBMaterial> mat, const std::string& path, const std::string& name = "defaultPrimitive");

        RBSceneNode* GetSelectedNode();
        RBSceneNode* SetSelectedNode(RBSceneNode* node);

        private:
        void OnNodeDestroyed(RBSceneNode* node);
        void RenderHierarchy(VkCommandBuffer& commandBuffer);  
        void RenderScene(VkCommandBuffer& commandBuffer);  
        RBSceneNode* m_SelectedNode = nullptr;
        RBDevice& rbDevice;
        RBCommandBuffer& rbCommandBuffer;
    };
}