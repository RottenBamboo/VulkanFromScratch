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
    void RBGUIFileMenu::RefreshSceneFiles()
    {
        m_SceneFiles.clear();
        const std::filesystem::path resourceRoot(GET_RESOURCE_ROOT_DIR + sceneFilePath);

        if (!std::filesystem::exists(resourceRoot))
        {
            return;
        }

        for (const auto& entry : std::filesystem::recursive_directory_iterator(resourceRoot))
        {
            if (!entry.is_regular_file())
            {
                continue;
            }

            const auto ext = entry.path().extension().string();
            if (ext == SCENE_EXTENSION)
            {
                std::filesystem::path rel = std::filesystem::relative(entry.path(), resourceRoot);
                m_SceneFiles.push_back(rel.generic_string());
            }
        }

        std::sort(m_SceneFiles.begin(), m_SceneFiles.end());
    }

    void RBGUIFileMenu::DrawOpenSceneBrowser()
    {
        if (!m_OpenSceneBrowser)
        {
            return;
        }

        ImGui::OpenPopup("Open Scene");
        if (ImGui::BeginPopupModal("Open Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            std::string selectedPath;

            if (m_SceneFiles.empty())
            {
                ImGui::Text("No .scene files found under resource/");
            }
            else
            {
                for (const auto& sceneFile : m_SceneFiles)
                {
                    if (ImGui::Selectable(sceneFile.c_str()))
                    {
                        selectedPath = sceneFile;
                    }
                }
            }

            ImGui::Separator();
            if (ImGui::Button("Close"))
            {
                m_OpenSceneBrowser = false;
                ImGui::CloseCurrentPopup();
            }

            if (!selectedPath.empty())
            {
                RBLOG_INFO("RBGUIFileMenu::OpenSceneBrowser selected: " + selectedPath);
                RBScene newScene(selectedPath);
                newScene.Load(selectedPath);
                RBApplication::instance->scene = std::move(newScene);
                RBApplication::ptr_scene = &RBApplication::instance->scene;
                m_OpenSceneBrowser = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    RBGUIFileMenu::RBGUIFileMenu(RBDevice& device) : rbDevice(device)
    {
    }

    void RBGUIFileMenu::Initialize(VkRenderPass renderPass)
    {
    }

    void RBGUIFileMenu::Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix)
    {
        SetLayout();
        DrawOpenSceneBrowser();
        
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
        }
        if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
        {
            RBLOG_INFO("RBGUIFileMenu::Open Scene");
            RefreshSceneFiles();
            m_OpenSceneBrowser = true;
        }
        if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
        {
            RBLOG_INFO("RBGUIFileMenu::Save Scene");
            RBScene* scene = RBApplication::GetScene();
            if (scene != nullptr)
            {
                scene->Save(sceneFilePath + defaultSceneName, scene->GetData(), scene->GetRoot());
            }
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit"))
        {
            m_ShouldClose = true;
        }
    }

}