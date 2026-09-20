
//
// Created by rottenbamboo on 2026/9/11.
//

#pragma once

#include "RBGUIBase.h"
#include "RBShaderDefinition.h"
#include "RBDescriptors.h"
#include <iostream>
namespace RottenBamboo 
{
    class RBGUIHierarchy : public RBGUIBase 
    {
    public:
        RBGUIHierarchy();

        virtual ~RBGUIHierarchy() override = default;

        virtual void Initialize(VkRenderPass renderPass) override;

        virtual void Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix) override;

        void SetLayout() override;

        private:
        void RenderHierarchy();

                
    };
}