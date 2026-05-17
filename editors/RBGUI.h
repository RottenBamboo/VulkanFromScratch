//
// Created by rottenbamboo on 2025/6/6.
//

#pragma once
#include "RBGUIMaterials.h"
namespace RottenBamboo 
{
    class RBGUI
    {
    
    public:

        VkDescriptorPool imguiDescriptorPool;    

        RBGUI(RBDevice &device, RBWindows &windows);

        virtual ~RBGUI();

        virtual void Initialize(VkRenderPass renderPass);

        virtual void Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix);

        virtual void SetEditorMaterial(RBMaterial* material);

        virtual void createDescriptorPool();

    public:
        void RenderGizmo(UniformBufferShaderVariables& uniformMatrix);

    private:
        RBDevice &rbDevice;
        RBWindows &rbWindows;
        RBGUIMaterials materialsGUI;
        bool showMaterialEditor = true;
        bool gizmoActive = true;
        ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
        ImGuizmo::MODE currentMode = ImGuizmo::WORLD;

    };

}