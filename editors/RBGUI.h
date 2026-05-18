//
// Created by rottenbamboo on 2025/6/6.
//

#pragma once
#include "RBGUIMaterials.h"
#include "RBGUIAssets.h"
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

        virtual void SetResourceManager(ResourceManager* resourceManager);

        virtual void createDescriptorPool();

    public:
        void RenderGizmo(UniformBufferShaderVariables& uniformMatrix);

    private:
        RBDevice &rbDevice;
        RBWindows &rbWindows;
        RBGUIMaterials materialsGUI;
        RBGUIAssets assetsGUI;
        bool materialEditorActive = true;
        bool gizmoActive = true;
        bool assetsActive = true;
        ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
        ImGuizmo::MODE currentMode = ImGuizmo::WORLD;

    };

}