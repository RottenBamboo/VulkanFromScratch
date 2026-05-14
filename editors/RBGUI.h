//
// Created by rottenbamboo on 2025/6/6.
//

#pragma once
#include "RBGUIBase.h"
namespace RottenBamboo 
{
    class RBGUI : public RBGUIBase
    {
    
    public:

        VkDescriptorPool imguiDescriptorPool;    

        RBGUI(RBDevice &device, RBWindows &windows);

        virtual ~RBGUI() override;

        virtual void Initialize(VkRenderPass renderPass);

        //void Reinitialize(SDL_Window* window, VkRenderPass renderPass);

        virtual void Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix);

        virtual void SetMaterialEditor(RBMaterial* material);

        virtual void createDescriptorPool();

    public:
        void RenderGizmo(UniformBufferShaderVariables& uniformMatrix);
        void RenderMaterialEditor();

    private:
        //RBDevice &rbDevice;
        //RBWindows &rbWindows;
        RBMaterial* materialEditor = nullptr;
        bool showMaterialEditor = true;
        bool gizmoActive = true;
        ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
        ImGuizmo::MODE currentMode = ImGuizmo::WORLD;

    };

}