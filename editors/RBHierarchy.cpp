
//
// Created by rottenbamboo on 2026/9/11.
//

#include "RBHierarchy.h"
namespace RottenBamboo 
{
    RBHierarchy::RBHierarchy(RBDevice& device) : rbDevice(device)
    {

    }

    void RBHierarchy::Initialize(VkRenderPass renderPass)
    {
        (void)renderPass;
    }

    void RBHierarchy::SetLayout()
    {

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 workPos = viewport->WorkPos;
        ImVec2 workSize = viewport->WorkSize;

        // Hierarchy Editor: left side panel
        float hierarchyWidth = 360.0f;
        ImGui::SetNextWindowPos(
            ImVec2(workPos.x, workPos.y),
            ImGuiCond_Always
        );
        ImGui::SetNextWindowSize(
            ImVec2(hierarchyWidth, workSize.y),
            ImGuiCond_Always
        );

        ImGui::Begin(
            "Hierarchy Editor",
            nullptr,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse
            );
    }

    void RBHierarchy::Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix)
    {
        (void)commandBuffer;
        (void)uniformMatrix;

        SetLayout();
        ImGui::End();
    }
}