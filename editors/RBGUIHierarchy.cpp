
//
// Created by rottenbamboo on 2026/9/11.
//

#include "RBGUIHierarchy.h"
#include "RBApplication.h"
#include <filesystem>
namespace fs = std::filesystem;
namespace RottenBamboo 
{
    RBGUIHierarchy::RBGUIHierarchy(RBDevice& device, RBCommandBuffer& commandBuffer)
        : rbDevice(device), rbCommandBuffer(commandBuffer)
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

        RenderHierarchy(commandBuffer);

        ImGui::End();
    }

    void RBGUIHierarchy::CreatePrimitive(SimplePrimitiveData* data, const std::string& path)
    {
        auto mesh = RBApplication::GetSimplePrimitive()->CreateSimplePrimitive(data);
        ResourceManager* resourceManager = RBApplication::GetResourceManager();
        std::string modelPath = GET_RESOURCE_ROOT_DIR + path + std::to_string(resourceManager->Count<RBModel>());
        modelPath = NormalizePathString(modelPath);
        auto res = resourceManager->Add<RBModel>(modelPath);
        res->addMesh(std::move(mesh));
    }
    void RBGUIHierarchy::RenderHierarchy(VkCommandBuffer& commandBuffer)
    {
        if (ImGui::BeginPopupContextWindow("HierarchyContext", ImGuiPopupFlags_MouseButtonRight))
        {
            if (ImGui::BeginMenu("Create Primitive"))
            {
                if (ImGui::MenuItem("Create Cube"))
                {
                    SimplePrimitiveData data;
                    data.type = SimplePrimitiveType::TYPE_CUBE;
                    data.width = 1;
                    data.height = 1;
                    CreatePrimitive(&data, "models/cube.gltf");
                }
                if (ImGui::MenuItem("Create Sphere"))
                {
                    SimplePrimitiveData data;
                    data.type = SimplePrimitiveType::TYPE_SPHERE;
                    data.radius = 1;
                    data.segments = 50;
                    data.rings = 50;
                    CreatePrimitive(&data, "models/sphere.gltf");
                }
                if (ImGui::MenuItem("Create Cylinder"))
                {
                    SimplePrimitiveData data;
                    data.type = SimplePrimitiveType::TYPE_CYLINDER;
                    data.width = 1;
                    data.height = 2;
                    CreatePrimitive(&data, "models/cylinder.gltf");
                }
                if (ImGui::MenuItem("Create Capsule"))
                {
                    SimplePrimitiveData data;
                    data.type = SimplePrimitiveType::TYPE_CAPSULE;
                    data.width = 1;
                    data.height = 2;
                    CreatePrimitive(&data, "models/capsule.gltf");
                }
                if (ImGui::MenuItem("Create Plane"))
                {
                    SimplePrimitiveData data;
                    data.type = SimplePrimitiveType::TYPE_PLANE;
                    data.width = 2;
                    data.height = 2;
                    CreatePrimitive(&data, "models/plane.gltf");
                }
                
                ImGui::EndMenu();
            }
            ImGui::EndPopup();
        }
    }
}