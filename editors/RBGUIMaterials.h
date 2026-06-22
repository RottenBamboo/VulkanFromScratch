
//
// Created by rottenbamboo on 2026/5/17.
//

#pragma once

#include "RBGUIBase.h"
#include "RBShaderDefinition.h"
#include <iostream>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_vulkan.h>
#include <ImGuizmo.h>
namespace RottenBamboo 
{
    class RBGUIMaterials : public RBGUIBase 
    {
    public:
        RBGUIMaterials();

        virtual ~RBGUIMaterials() override = default;

        virtual void Initialize(VkRenderPass renderPass) override;

        virtual void Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix) override;

        void SetMaterial(RBMaterial* material);
        
        RBMaterial* currentMaterial = nullptr;
        
        std::string currentSelectedShader;

        RBShaderDefinition shaderDefinition;

        const ImVec2 imageVec = ImVec2(64, 64);
        std::vector<VkDescriptorSet> vecUIMaterialDescriptorSet;
    };
}