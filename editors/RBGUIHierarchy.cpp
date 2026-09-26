
//
// Created by rottenbamboo on 2026/9/11.
//

#include "RBGUIHierarchy.h"
#include "RBApplication.h"
#include <filesystem>
namespace fs = std::filesystem;
namespace RottenBamboo 
{
    RBGUIHierarchy::RBGUIHierarchy(RBDevice& device)
        : rbDevice(device)
    {

    }

    void RBGUIHierarchy::Initialize(VkRenderPass renderPass)
    {
        (void)renderPass;
    }

    void RBGUIHierarchy::SetLayout()
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

    void RBGUIHierarchy::Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix)
    {
        (void)commandBuffer;
        (void)uniformMatrix;

        SetLayout();

        RenderHierarchy();

        ImGui::End();
    }

    void RBGUIHierarchy::RenderHierarchy()
    {
        if (ImGui::BeginPopupContextWindow("HierarchyContext", ImGuiPopupFlags_MouseButtonRight))
        {
            if (ImGui::MenuItem("Create Cube"))
            {
                auto mesh = RBApplication::GetSimplePrimitive()->CreateCube(5);
                std::string modelPath = GET_RESOURCE_ROOT_DIR + "models/mach.gltf";
                modelPath = NormalizePathString(modelPath);
                std::shared_ptr<RBModel> model = RBApplication::GetResourceManager()->Get<RBModel>(modelPath);
                if(model)
                {
                    int meshCount = model->getMeshCount();
                    for(int i = 0; i < meshCount; i++)
                    {
                        std::unique_ptr<RBMesh>& meshes = model->getMeshes(i);
                        if(meshes)
                        {
                            model->setMeshes(i, std::move(mesh));
                        }
                    }
                }
                
            }
            if (ImGui::MenuItem("Create Sphere"))
            {

            }
            ImGui::EndPopup();
        }
    }
}