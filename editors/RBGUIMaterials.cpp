#include "RBGUIMaterials.h"
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
    
    RBGUIMaterials::RBGUIMaterials()
    {
    }

    void RBGUIMaterials::Initialize(VkRenderPass renderPass)
    {
    }

    void RBGUIMaterials::SetMaterial(RBMaterial* material)
    {
        this->currentMaterial = material;
    }

    void RBGUIMaterials::Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix)
    {
        if (currentMaterial == nullptr)
        {
            return;
        }

        MaterialData& material = currentMaterial->GetData();

        ImGui::Begin("Material Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        char filePathBuffer[512];
        CopyStringToBuffer(filePathBuffer, sizeof(filePathBuffer), currentMaterial->GetPath());

        currentMaterial->SetPath(materialsFilePath.c_str());

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
            currentMaterial->Reset();
        }
        ImGui::SameLine();
        if (ImGui::Button("Load"))
        {
            currentMaterial->Load(currentMaterial->GetPath() + material.name + ".mat");
        }
        ImGui::SameLine();
        if (ImGui::Button("Save"))
        {
            currentMaterial->Save(currentMaterial->GetPath() + material.name + ".mat");
        }

        ImGui::TextWrapped("Edit the material properties and save them as .mat files.");
        ImGui::End();
    }
}