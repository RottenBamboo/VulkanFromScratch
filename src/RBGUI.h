//
// Created by rottenbamboo on 2025/6/6.
//

#pragma once

#include "RBDevice.h"
#include "RBMaterial.h"
#include <iostream>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_vulkan.h>
#include <ImGuizmo.h>
namespace RottenBamboo 
{
    class RBGUI 
    {
    
    public:

        VkDescriptorPool imguiDescriptorPool;    

        RBGUI(RBDevice &device, RBWindows &windows);

        ~RBGUI();

        void Initialize(VkRenderPass renderPass);

        //void Reinitialize(SDL_Window* window, VkRenderPass renderPass);

        void Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix);

        void SetMaterialEditor(RBMaterial* material);

        void createDescriptorPool();

    public:
        void RenderGizmo(UniformBufferShaderVariables& uniformMatrix);
        void RenderMaterialEditor();

    private:
        RBDevice &rbDevice;
        RBWindows &rbWindows;
        RBMaterial* materialEditor = nullptr;
        bool showMaterialEditor = true;
        bool gizmoActive = true;
        ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
        ImGuizmo::MODE currentMode = ImGuizmo::WORLD;

    };

}