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

        VkDescriptorPool imguiDescriptorPool;    

        RBGUIBase(RBDevice &device, RBWindows &windows) : rbDevice(device), rbWindows(windows) {};

        virtual ~RBGUIBase() = default;

        virtual void Initialize(VkRenderPass renderPass) = 0;

        virtual void Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix) = 0;

        virtual void createDescriptorPool() = 0;

    public:
        virtual void RenderGizmo(UniformBufferShaderVariables& uniformMatrix) = 0;

    private:
    protected:
        RBDevice &rbDevice;
        RBWindows &rbWindows;
        //RBMaterial* materialEditor = nullptr;
        //bool showMaterialEditor = true;
        //bool gizmoActive = true;
        //ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
        //ImGuizmo::MODE currentMode = ImGuizmo::WORLD;

    };

}