//
// Created by rottenbamboo on 2025/6/6.
//

#pragma once

#include "RBDevice.h"
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

        RBGUI(RBDevice &device);

        ~RBGUI();

        void Initialize(SDL_Window* window, VkRenderPass renderPass);

        //void Reinitialize(SDL_Window* window, VkRenderPass renderPass);

        void Render(VkCommandBuffer& commandBuffer, UniformBufferObject& uniformMatrix);

        void createDescriptorPool();

    public:
        void RenderGizmo(UniformBufferObject& uniformMatrix);

    private:
        RBDevice &rbDevice;
        bool gizmoActive = true;
        ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
        ImGuizmo::MODE currentMode = ImGuizmo::WORLD;

    };

}