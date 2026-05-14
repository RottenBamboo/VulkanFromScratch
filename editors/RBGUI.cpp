//
// Created by rottenbamboo on 2025/6/6.
//

#include "RBGUI.h"
#include <glm/gtc/type_ptr.hpp>
#include <cstring>

namespace RottenBamboo
{
    static void CopyStringToBuffer(char* buffer, size_t bufferSize, const std::string& value)
    {
        if (bufferSize == 0) {
            return;
        }

        std::strncpy(buffer, value.c_str(), bufferSize - 1);
        buffer[bufferSize - 1] = '\0';
    }

    void RBGUI::RenderGizmo(UniformBufferShaderVariables &uniformMatrix)
    {
        ImGuizmo::Enable(true);

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::BeginFrame();
        ImVec2 mouse = ImGui::GetMousePos();
        ImGui::GetForegroundDrawList()->AddCircle(mouse, 5.0f, IM_COL32(255, 0, 0, 255), 32, 2.0f);
        ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());
        ImGuiIO &io = ImGui::GetIO();
        ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

        static ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
        static ImGuizmo::MODE mode = ImGuizmo::WORLD;

        glm::mat4 view = uniformMatrix.view;
        glm::mat4 proj = uniformMatrix.proj;
        glm::mat4 model = uniformMatrix.model;
        proj[1][1] *= -1.0f;

        ImGuizmo::Manipulate(glm::value_ptr(view),
                             glm::value_ptr(proj),
                             operation,
                             mode,
                             glm::value_ptr(model));

        if (ImGuizmo::IsUsing())
        {
            uniformMatrix.model = model;
        }

        ImGui::Begin("Gizmo Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::RadioButton("Translate", (int *)&operation, ImGuizmo::TRANSLATE);
        ImGui::SameLine();
        ImGui::RadioButton("Rotate", (int *)&operation, ImGuizmo::ROTATE);
        ImGui::SameLine();
        ImGui::RadioButton("Scale", (int *)&operation, ImGuizmo::SCALE);

        ImGui::RadioButton("World", (int *)&mode, ImGuizmo::WORLD);
        ImGui::SameLine();
        ImGui::RadioButton("Local", (int *)&mode, ImGuizmo::LOCAL);
        ImGui::Checkbox("Material Editor", &showMaterialEditor);
        ImGui::End();
    }

    void RBGUI::RenderMaterialEditor()
    {
        if (materialEditor == nullptr)
        {
            return;
        }

        MaterialData& material = materialEditor->GetData();

        ImGui::Begin("Material Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        char filePathBuffer[512];
        CopyStringToBuffer(filePathBuffer, sizeof(filePathBuffer), materialEditor->GetPath());
        if (ImGui::InputText("Save Path", filePathBuffer, sizeof(filePathBuffer)))
        {
            materialEditor->SetPath(filePathBuffer);
        }

        char nameBuffer[128];
        CopyStringToBuffer(nameBuffer, sizeof(nameBuffer), material.name);
        if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer)))
        {
            material.name = nameBuffer;
        }

        ImGui::ColorEdit4("Base Color", material.baseColor.data());
        ImGui::ColorEdit3("Emissive Color", material.emissiveColor.data());
        ImGui::SliderFloat("Metallic", &material.metallic, 0.0f, 1.0f);
        ImGui::SliderFloat("Roughness", &material.roughness, 0.0f, 1.0f);
        ImGui::SliderFloat("Emissive Strength", &material.emissiveStrength, 0.0f, 50.0f);
        ImGui::SliderFloat("Alpha Cutoff", &material.alphaCutoff, 0.0f, 1.0f);

        char albedoBuffer[512];
        char normalBuffer[512];
        char mrBuffer[512];
        char emissiveBuffer[512];

        CopyStringToBuffer(albedoBuffer, sizeof(albedoBuffer), material.baseColorTexture);
        CopyStringToBuffer(normalBuffer, sizeof(normalBuffer), material.normalTexture);
        CopyStringToBuffer(mrBuffer, sizeof(mrBuffer), material.metallicRoughnessTexture);
        CopyStringToBuffer(emissiveBuffer, sizeof(emissiveBuffer), material.emissiveTexture);

        if (ImGui::InputText("Base Color Texture", albedoBuffer, sizeof(albedoBuffer)))
        {
            material.baseColorTexture = albedoBuffer;
        }
        if (ImGui::InputText("Normal Texture", normalBuffer, sizeof(normalBuffer)))
        {
            material.normalTexture = normalBuffer;
        }
        if (ImGui::InputText("Metallic Roughness Texture", mrBuffer, sizeof(mrBuffer)))
        {
            material.metallicRoughnessTexture = mrBuffer;
        }
        if (ImGui::InputText("Emissive Texture", emissiveBuffer, sizeof(emissiveBuffer)))
        {
            material.emissiveTexture = emissiveBuffer;
        }

        if (ImGui::Button("Reset"))
        {
            materialEditor->Reset();
        }
        ImGui::SameLine();
        if (ImGui::Button("Load"))
        {
            materialEditor->Load(materialEditor->GetPath());
        }
        ImGui::SameLine();
        if (ImGui::Button("Save"))
        {
            materialEditor->Save(materialEditor->GetPath());
        }
        ImGui::SameLine();
        if (ImGui::Button("Save As Default"))
        {
            materialEditor->Save(GET_PROJECT_ROOT_DIR + std::string("materials/default_material.json"));
        }

        ImGui::TextWrapped("Edit the material properties and save them as JSON.");
        ImGui::End();
    }

    void RBGUI::SetMaterialEditor(RBMaterial* material)
    {
        materialEditor = material;
    }

    RBGUI::RBGUI(RBDevice &device, RBWindows &window) : RBGUIBase(device, window)
    {
        checkbox = false;
        std::cout << "RBGUI::RBGUI()" << std::endl;
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
        imguiDescriptorPool = VK_NULL_HANDLE;
    }

    RBGUI::~RBGUI()
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        
        if (imguiDescriptorPool != VK_NULL_HANDLE)
        {
            vkDestroyDescriptorPool(rbDevice.device, imguiDescriptorPool, nullptr);
            imguiDescriptorPool = VK_NULL_HANDLE;
        }
        std::cout << "RBGUI::~RBGUI()" << std::endl;
    }

    void RBGUI::createDescriptorPool()
    {
        if (imguiDescriptorPool != VK_NULL_HANDLE)
        {
            vkDestroyDescriptorPool(rbDevice.device, imguiDescriptorPool, nullptr);
            imguiDescriptorPool = VK_NULL_HANDLE;
        }
        VkDescriptorPoolSize poolSizes[] =
            {
                {VK_DESCRIPTOR_TYPE_SAMPLER, 100},
                {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100},
                {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100},
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 100},
            };

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1000;
        poolInfo.poolSizeCount = static_cast<uint32_t>(std::size(poolSizes));
        poolInfo.pPoolSizes = poolSizes;

        vkCreateDescriptorPool(rbDevice.device, &poolInfo, nullptr, &imguiDescriptorPool);
    }

    void RBGUI::Initialize(VkRenderPass renderPass)
    {
        createDescriptorPool();

        ImGui_ImplSDL3_InitForVulkan(rbWindows.window);

        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = rbDevice.instance;
        init_info.PhysicalDevice = rbDevice.physicalDevice;
        init_info.Device = rbDevice.device;
        init_info.QueueFamily = rbDevice.findQueueFamilies(rbDevice.physicalDevice).graphicsFamily.value();
        init_info.Queue = rbDevice.graphicsQueue;
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = imguiDescriptorPool;
        init_info.MinImageCount = MAX_FRAMES_IN_FLIGHT;
        init_info.ImageCount = MAX_FRAMES_IN_FLIGHT;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.RenderPass = renderPass;
        init_info.Subpass = 0;

        ImGui_ImplVulkan_Init(&init_info);
    }
    void RBGUI::Render(VkCommandBuffer &commandBuffer, UniformBufferShaderVariables &uniformMatrix)
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        //ImGui::ShowMetricsWindow();
        RenderGizmo(uniformMatrix);
        if (showMaterialEditor)
        {
            RenderMaterialEditor();
        }
        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
    }
}