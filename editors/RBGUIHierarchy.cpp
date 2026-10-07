//
// Created by rottenbamboo on 2026/9/11.
//

#include "RBGUIHierarchy.h"
#include "RBApplication.h"
#include <filesystem>
#include <functional>
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
        RenderScene(commandBuffer);
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
    void RBGUIHierarchy::RenderScene(VkCommandBuffer& commandBuffer)
    {
        (void)commandBuffer;

        auto* scene = RBApplication::GetScene();
        if (!scene) return;

        RBSceneNode* root = scene->GetRoot();
        if (!root) return;

        std::function<void(RBSceneNode*)> drawNode = [&](RBSceneNode* node)
        {
            if (!node) return;

            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
                                       ImGuiTreeNodeFlags_SpanAvailWidth;

            if (node->m_Children.empty())
                flags |= ImGuiTreeNodeFlags_Leaf;

            const bool opened = ImGui::TreeNodeEx(
                node,
                flags,
                "%s",
                node->GetName().c_str()
            );

            if (opened)
            {
                for (auto& child : node->m_Children)
                {
                    if (child)
                        drawNode(child.get());
                }

                ImGui::TreePop();
            }
        };

        drawNode(root);
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
                    RBApplication::instance->AddDefaultMaterial();
                }
                if (ImGui::MenuItem("Create Sphere"))
                {
                    SimplePrimitiveData data;
                    data.type = SimplePrimitiveType::TYPE_SPHERE;
                    data.radius = 1;
                    data.segments = 50;
                    data.rings = 50;
                    CreatePrimitive(&data, "models/sphere.gltf");
                    RBApplication::instance->AddDefaultMaterial();
                }
                if (ImGui::MenuItem("Create Cylinder"))
                {
                    SimplePrimitiveData data;
                    data.type = SimplePrimitiveType::TYPE_CYLINDER;
                    data.width = 1;
                    data.height = 2;
                    CreatePrimitive(&data, "models/cylinder.gltf");
                    RBApplication::instance->AddDefaultMaterial();
                }
                if (ImGui::MenuItem("Create Capsule"))
                {
                    SimplePrimitiveData data;
                    data.type = SimplePrimitiveType::TYPE_CAPSULE;
                    data.radius = 1;
                    data.height = 2;
                    data.segments = 50;
                    data.rings = 50;
                    CreatePrimitive(&data, "models/capsule.gltf");
                    RBApplication::instance->AddDefaultMaterial();
                }
                if (ImGui::MenuItem("Create Plane"))
                {
                    SimplePrimitiveData data;
                    data.type = SimplePrimitiveType::TYPE_PLANE;
                    data.width = 2;
                    data.height = 2;
                    CreatePrimitive(&data, "models/plane.gltf");
                    RBApplication::instance->AddDefaultMaterial();
                }
                
                ImGui::EndMenu();
            }
            ImGui::EndPopup();
        }
    }
}