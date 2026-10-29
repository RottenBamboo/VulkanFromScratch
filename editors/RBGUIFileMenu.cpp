//
// Created by rottenbamboo on 2026/10/29.
//

#include "RBGUIFileMenu.h"
#include "RBApplication.h"
#include <filesystem>
#include <functional>
#include <memory>
namespace fs = std::filesystem;
namespace RottenBamboo 
{
    RBGUIFileMenu::RBGUIFileMenu(RBDevice& device) : rbDevice(device)
    {
    }

    void RBGUIFileMenu::Initialize(VkRenderPass renderPass)
    {
    }

    void RBGUIFileMenu::Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix)
    {
        SetLayout();
        
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File"))
        {
            DrawContent();
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    
    void RBGUIFileMenu::RenderFileToolbar(VkCommandBuffer& commandBuffer)
    {
    }
    
    void RBGUIFileMenu::SetLayout()
    {
    }
    void RBGUIFileMenu::DrawContent()
    {
        if (ImGui::MenuItem("New Scene", "Ctrl+N"))
        {
            RBLOG_INFO("RBGUIFileMenu::New Scene");
            // if (RBApplication::GetScene() != nullptr)
            // {
            //     RBApplication::GetScene()->SetRoot(std::make_unique<RBSceneNode>("Scene"));
            // }
        }
        if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
        {
            RBLOG_INFO("RBGUIFileMenu::Open Scene");
        }
        if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
        {
            RBLOG_INFO("RBGUIFileMenu::Save Scene");
            RBScene* scene = RBApplication::GetScene();
            if (scene != nullptr)
            {
            }
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit"))
        {
            m_ShouldClose = true;
        }
    }

}