
//
// Created by rottenbamboo on 2026/5/19.
//

#include "RBGUIBase.h"
#include <iostream>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_vulkan.h>
#include <ImGuizmo.h>
namespace RottenBamboo 
{
    class RBGUIAssets : public RBGUIBase 
    {
    public:
        RBGUIAssets();
        
        virtual ~RBGUIAssets() override = default;

        virtual void Initialize(VkRenderPass renderPass) override;

        virtual void Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix) override;
    };
}