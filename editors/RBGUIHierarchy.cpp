//
// Created by rottenbamboo on 2026/9/11.
//

#include "RBGUIHierarchy.h"
#include "RBApplication.h"
#include "RBMaterial.h"
#include "RBModel.h"
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

        PopupMenu(commandBuffer, ClickFrom::CLICK_FROM_HIERARCHY);
        RenderScene(commandBuffer);
        ImGui::End();
    }

    void RBGUIHierarchy::CreateModel(std::shared_ptr<RBMaterial> mat, const std::string& modelPath, const std::string& name, RBSceneNode* parentNode)
    {
        ResourceManager* resourceManager = RBApplication::GetResourceManager();
        std::string count = std::to_string(resourceManager->Count<RBModel>());
        std::string primitivePath = GET_RESOURCE_ROOT_DIR + modelPath;
        primitivePath = NormalizePathString(primitivePath);
        auto res = resourceManager->Load<RBModel>(primitivePath);
        //res->addMesh(std::move(mesh));

        auto* scene = RBApplication::GetScene();
        if (!scene) return;

        RBSceneNode* parent = parentNode ? parentNode : scene->GetRoot();
        if (!parent) return;

        RBSceneNode* node = parent->CreateChild(name + count);

        vector3 pos(0, 0, 0);
        vector3 scale(1, 1, 1);
        quat rot(1, 0, 0, 0);
        if (node)
        {
            node->m_Resource = res.get();
            node->m_Model = res;
            node->m_Material = mat;
            node->m_Transform = Transform(pos, scale, rot);
            node->SetDescriptorOffset(mat->GetDescriptorOffset());
        }
    }
    void RBGUIHierarchy::DeletePrimitive(RBSceneNode* node)
    {
        if (node && node != RBApplication::GetScene()->GetRoot())
        {
            node->Release();
        }
    }

    void RBGUIHierarchy::CreatePrimitive(SimplePrimitiveData* data, std::shared_ptr<RBMaterial> mat, const std::string& modelPath, const std::string& name, RBSceneNode* parentNode)
    {
        auto mesh = RBApplication::GetSimplePrimitive()->CreateSimplePrimitive(data);
        ResourceManager* resourceManager = RBApplication::GetResourceManager();
        std::string count = std::to_string(resourceManager->Count<RBModel>());
        std::string primitivePath = GET_RESOURCE_ROOT_DIR + modelPath + count;
        primitivePath = NormalizePathString(primitivePath);
        auto res = resourceManager->Add<RBModel>(primitivePath);
        res->addMesh(std::move(mesh));

        auto* scene = RBApplication::GetScene();
        if (!scene) return;

        RBSceneNode* parent = parentNode ? parentNode : scene->GetRoot();
        if (!parent) return;

        RBSceneNode* node = parent->CreateChild(name + count);

        vector3 pos(0, 0, 0);
        vector3 scale(1, 1, 1);
        quat rot(1, 0, 0, 0);
        if (node)
        {
            node->m_Resource = res.get();
            node->m_Model = res;
            node->m_Material = mat;
            node->m_Transform = Transform(pos, scale, rot);
            node->SetDescriptorOffset(mat->GetDescriptorOffset());
        }
    }
    void RBGUIHierarchy::RenderScene(VkCommandBuffer& commandBuffer)
    {
        (void)commandBuffer;

        auto* scene = RBApplication::GetScene();
        if (!scene) return;

        RBSceneNode* root = scene->GetRoot();
        if (!root) return;

        bool droppedOnNode = false;

        std::function<void(RBSceneNode*)> drawNode = [&](RBSceneNode* node)
        {
            if (!node) return;

            ImGui::PushID(node);

            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
                                       ImGuiTreeNodeFlags_SpanAvailWidth;

            if (node == m_SelectedNode)
            {
                flags |= ImGuiTreeNodeFlags_Selected;
            }

            if (node->m_Children.empty())
                flags |= ImGuiTreeNodeFlags_Leaf;

            const bool opened = ImGui::TreeNodeEx(
                node,
                flags,
                "%s",
                node->GetName().c_str()
            );

            if (ImGui::IsItemClicked())
            {
                SetSelectedNode(node);
            }

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_FILE")) 
                {
                    const char* filePath = static_cast<const char*>(payload->Data);
                    std::string path(filePath, payload->DataSize);
                    std::string assetPath = NormalizePathString(path);
                
                    std::string ext = std::filesystem::path(assetPath).extension().string();
                    if (ext == ".gltf" || ext == ".glb" || ext == ".fbx" || ext == ".obj")
                    {
                        std::shared_ptr<RBMaterial> material = RBApplication::instance->AddMaterial();
                        CreateModel(material, assetPath, assetPath, node);
                        droppedOnNode = true;
                    }
                }
                ImGui::EndDragDropTarget();
            }

            if (opened)
            {
                for (auto& child : node->m_Children)
                {
                    if (child)
                        drawNode(child.get());
                }

                ImGui::TreePop();
            }

            //render right click menu
            PopupMenu(commandBuffer, ClickFrom::CLICK_FROM_NODE, node);

            ImGui::PopID();
        };

        drawNode(root);

        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0) && !ImGui::IsAnyItemHovered()) 
        {
            SetSelectedNode(nullptr);
        }

        ImVec2 dropSize = ImGui::GetContentRegionAvail();
        ImGui::InvisibleButton("HierarchyDropTarget", dropSize);

        if (ImGui::BeginDragDropTarget() && !ImGui::IsAnyItemHovered())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_FILE")) 
            {
                const char* filePath = static_cast<const char*>(payload->Data);
                std::string path(filePath, payload->DataSize);
                std::string assetPath = NormalizePathString(path);

                std::string ext = std::filesystem::path(assetPath).extension().string();
                if (ext == ".gltf" || ext == ".glb" || ext == ".fbx" || ext == ".obj")
                {
                    std::shared_ptr<RBMaterial> material = RBApplication::instance->AddMaterial();
                    CreateModel(material, assetPath, assetPath);
                }
            }
            ImGui::EndDragDropTarget();
        }
    }

    RBSceneNode* RBGUIHierarchy::SetSelectedNode(RBSceneNode* node)
    {
        m_SelectedNode = node;
        return m_SelectedNode;
    }
    RBSceneNode* RBGUIHierarchy::GetSelectedNode()
    {
        return m_SelectedNode;
    }
    
    void RBGUIHierarchy::OnNodeDestroyed(RBSceneNode* node) 
    {
        if (m_SelectedNode == node) 
        {
            m_SelectedNode = nullptr;
        }
    }

    void RBGUIHierarchy::RenderMenu(VkCommandBuffer& commandBuffer, RBSceneNode* node)
    {
        if (ImGui::BeginMenu("Create Primitive"))
        {
            if (ImGui::MenuItem("Create Cube"))
            {
                SimplePrimitiveData data;
                data.type = SimplePrimitiveType::TYPE_CUBE;
                data.width = 1;
                data.height = 1;
                std::shared_ptr<RBMaterial> material = RBApplication::instance->AddMaterial();
                CreatePrimitive(&data, material, "models/cube.gltf", "cube", node);
            }
            if (ImGui::MenuItem("Create Sphere"))
            {
                SimplePrimitiveData data;
                data.type = SimplePrimitiveType::TYPE_SPHERE;
                data.radius = 0.5f;
                data.segments = 50;
                data.rings = 50;
                std::shared_ptr<RBMaterial> material = RBApplication::instance->AddMaterial();
                CreatePrimitive(&data, material, "models/sphere.gltf", "sphere", node);
            }
            if (ImGui::MenuItem("Create Cylinder"))
            {
                SimplePrimitiveData data;
                data.type = SimplePrimitiveType::TYPE_CYLINDER;
                data.width = 0.5f;
                data.height = 1;
                std::shared_ptr<RBMaterial> material = RBApplication::instance->AddMaterial();
                CreatePrimitive(&data, material, "models/cylinder.gltf", "cylinder");
            }
            if (ImGui::MenuItem("Create Capsule"))
            {
                SimplePrimitiveData data;
                data.type = SimplePrimitiveType::TYPE_CAPSULE;
                data.radius = 0.5f;
                data.height = 1;
                data.segments = 50;
                data.rings = 50;
                std::shared_ptr<RBMaterial> material = RBApplication::instance->AddMaterial();
                CreatePrimitive(&data, material, "models/capsule.gltf", "capsule", node);
            }
            if (ImGui::MenuItem("Create Plane"))
            {
                SimplePrimitiveData data;
                data.type = SimplePrimitiveType::TYPE_PLANE;
                data.width = 2;
                data.height = 2;
                std::shared_ptr<RBMaterial> material = RBApplication::instance->AddMaterial();
                CreatePrimitive(&data, material, "models/plane.gltf", "plane", node);
            }
            
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Delete"))
        {
            DeletePrimitive(node);
        }
            
        ImGui::EndPopup();
    }

    void RBGUIHierarchy::PopupMenu(VkCommandBuffer& commandBuffer, ClickFrom clickFrom, RBSceneNode* node)
    {
        //popup from node right click
        if(clickFrom == ClickFrom::CLICK_FROM_NODE)
        {
            
            if (ImGui::BeginPopupContextItem("node_context", ImGuiPopupFlags_MouseButtonRight))
            {
                RenderMenu(commandBuffer, node);
            }
        }

        //popup from hierarchy right click
        if(clickFrom == ClickFrom::CLICK_FROM_HIERARCHY)
        {
            if (ImGui::BeginPopupContextWindow("hierarchy_context", ImGuiPopupFlags_MouseButtonRight))
            {
                RenderMenu(commandBuffer);
            }
        }
    }
}