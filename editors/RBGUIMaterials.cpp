#include "RBGUIMaterials.h"
#include "RBApplication.h"
#include <glm/gtc/type_ptr.hpp>
#include <cstring>
namespace RottenBamboo
{
    static std::vector<std::string> CollectShaders(const std::string& shaderDir)
    {
        std::vector<std::string> out;
        namespace fs = std::filesystem;

        std::error_code ec;
        if (!fs::exists(shaderDir, ec))
        {
            return out;
        }

        for (const auto& entry : fs::directory_iterator(shaderDir, ec))
        {
            if (ec || !entry.is_regular_file()) continue;

            const auto ext = entry.path().extension().string();
            if (ext == ".spv")
            {
                out.push_back(entry.path().filename().string());
            }
        }

        std::sort(out.begin(), out.end());
        return out;
    }
    static std::vector<std::string> CollectReflectedShaders()
    {
        CustomShaderReflectionMap customShaderReflection = RBApplication::GetResourceShader()->GetCustomReflection();
        int shaderCount = customShaderReflection.size();
        std::vector<std::string> out;
        out.resize(shaderCount);
        auto itr = customShaderReflection.begin();
        for(int i = 0; itr != customShaderReflection.end(); itr++, i++)
        {
            out[i] = (*itr).first;
        }

        std::sort(out.begin(), out.end());
        return out;
    }
    
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
        std::vector<RBDescriptors*>* ptr_Descriptors = RBApplication::GetDescriptors();
        int imageCount = (*ptr_Descriptors)[0]->rbImageManager.imageBundles.size();
        vecUIMaterialDescriptorSet.clear();
        vecUIMaterialDescriptorSet.reserve(imageCount);
        for(int i = 0; i < imageCount; i++)
        {
            vecUIMaterialDescriptorSet.push_back(ImGui_ImplVulkan_AddTexture((*ptr_Descriptors)[0]->rbImageManager.imageBundles[i].sampler, (*ptr_Descriptors)[0]->rbImageManager.imageBundles[i].imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
        }
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

        static std::vector<std::string> shaderList;

        //ImGui::Text("Current Shader: %s", material.shaderName.empty() ? "<None>" : material.shaderName.c_str());
        char shaderPathBuffer[128];
        CopyStringToBuffer(shaderPathBuffer, sizeof(shaderPathBuffer), material.shaderPathName);
        if (ImGui::InputText("shaderPathName", shaderPathBuffer, sizeof(shaderPathBuffer)))
        {
            material.shaderPathName = NormalizePathString(shaderPathBuffer);
        }
        
        ImGui::SameLine();
        if (ImGui::Button("Select Shader..."))
        {
            shaderList = CollectReflectedShaders();
            ImGui::OpenPopup("Shader Picker");
        }

        if (ImGui::BeginPopupModal("Shader Picker", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (ImGui::Button("Refresh"))
            {
                shaderList = CollectReflectedShaders();
            }

            ImGui::Separator();
            ImGui::BeginChild("ShaderList", ImVec2(520, 260), true);

            if (shaderList.empty())
            {
                ImGui::TextDisabled("No shader files found.");
            }
            else
            {
                for (const auto& s : shaderList)
                {
                    const bool isSelected = (currentSelectedShader == s);
                    
                     if (ImGui::Selectable(s.c_str(), isSelected))
                    {
                        currentSelectedShader = s;
                        material.shaderReflection = RBApplication::GetResourceShader()->GetCustomReflection(s);
                        material.shaderPathName = NormalizePathString(s);
                        ImGui::CloseCurrentPopup();
                    }
                }
            }

            ImGui::EndChild();

            if (ImGui::Button("Close"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }


        if(material.shaderReflection && material.shaderReflection->descriptorSets.size() > 0)
        {
            auto& descriptorSets = material.shaderReflection->descriptorSets.at(0);
            int i = 0;
            for(auto itr = descriptorSets.bindings.begin(); itr != descriptorSets.bindings.end(); itr++)
            {
                if(itr->second.type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
                {
                    continue;
                }
                float temp = 0.5f;
                const std::string &name = itr->second.name;
                float value = itr->second.floatValue;
                if(itr->second.type & (VK_DESCRIPTOR_TYPE_SAMPLER | 
                                        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER | 
                                        VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE ))
                {
                    ImGui::Image((ImTextureID)vecUIMaterialDescriptorSet[i], imageVec);
                    i++;
                }
                else
                {
                    ImGui::SliderFloat(name.c_str(), &value, 0, 1);
                }
                itr->second.floatValue = value;
            }
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
            currentMaterial->Save(currentMaterial->GetPath() + material.name + ".mat", material);
        }

        ImGui::TextWrapped("Edit the material properties and save them as .mat files.");
        ImGui::End();
    }
}