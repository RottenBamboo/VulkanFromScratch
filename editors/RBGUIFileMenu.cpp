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


    void RBGUIFileMenu::DrawPopupInputSceneName()
    {
        if (!m_NewScenePopupOpen)
            return;

        ImGui::OpenPopup("New Scene Name");
        if (ImGui::BeginPopupModal("New Scene Name", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::TextUnformatted("Name");
            if (ImGui::IsWindowAppearing())
                ImGui::SetKeyboardFocusHere();

            ImGui::InputText("##newscene", m_NewSceneFileNameBuf, sizeof(m_NewSceneFileNameBuf));
            m_NewSceneFileName = m_NewSceneFileNameBuf;

            if (ImGui::Button("Save", ImVec2(100, 0)) && m_NewSceneFileNameBuf[0] != '\0')
            {
                std::string fileName = m_NewSceneFileName;

                RBScene* scene = RBApplication::GetScene();
                if (scene != nullptr)
                {
                    if(scene->GetRoot())
                    {
                        scene->GetRoot()->SetName(fileName);
                    }
                    scene->Save(sceneFilePath + fileName,
                                scene->GetData(), scene->GetRoot());
                }
                m_NewScenePopupOpen = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(100, 0)))
            {
                m_NewScenePopupOpen = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
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
        DrawPopupInputSceneName();
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
            m_NewSceneFileName.clear();
            m_NewSceneFileNameBuf[0] = '\0';
            m_NewScenePopupOpen = true;
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
                std::string sceneName = scene->GetRoot()->GetName();
                scene->GetData().name = sceneName;
                scene->Save(sceneFilePath + sceneName, scene->GetData(), scene->GetRoot());
            }
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit"))
        {
            m_ShouldClose = true;
        }
    }

}