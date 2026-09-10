
//
// Created by rottenbamboo on 2026/9/11.
//

#pragma once

#include "RBGUIBase.h"
#include "RBShaderDefinition.h"
#include "RBDescriptors.h"
#include <iostream>
// #include <imgui.h>
// #include <imgui_impl_sdl3.h>
// #include <imgui_impl_vulkan.h>
// #include <ImGuizmo.h>
namespace RottenBamboo 
{
    class RBHierarchy : public RBGUIBase 
    {
    public:
        RBHierarchy(RBDevice& device);

        virtual ~RBHierarchy() override = default;

        virtual void Initialize(VkRenderPass renderPass) override;

        virtual void Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix) override;
        
        RBDevice& rbDevice;
    };
}