//
// Created by rottenbamboo on 2025/6/6.
//

#pragma once

#include "../src/RBDevice.h"
#include "../src/RBMaterial.h"
#include <iostream>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_vulkan.h>
#include <ImGuizmo.h>
namespace RottenBamboo 
{
    class RBGUIBase 
    {
    public:
        RBGUIBase() {};

        virtual ~RBGUIBase() = default;

        virtual void Initialize(VkRenderPass renderPass) = 0;

        virtual void Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix) = 0;
    };

}