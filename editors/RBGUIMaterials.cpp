//
// Created by rottenbamboo on 2026/5/17.
//
#include "RBGUIMaterials.h"
#include "RBApplication.h"
#include <glm/gtc/type_ptr.hpp>
#include <cstring>
namespace fs = std::filesystem;
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
    
    static std::vector<std::string> CollectionShaderDefinition()
    {
        std::unordered_map<std::string, RBShaderDefinition>* shaderDefinition = RBApplication::GetShaderDefinition();
        int shaderDefinitionCount = (*shaderDefinition).size();
        std::vector<std::string> out;
        out.resize(shaderDefinitionCount);
        auto itr = shaderDefinition->begin();
        for(int i = 0; itr != shaderDefinition->end(); itr++, i++)
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
    
    RBGUIMaterials::RBGUIMaterials(RBDevice& device) : rbDevice(device)
    {
    }

    void RBGUIMaterials::Initialize(VkRenderPass renderPass)
    {
    }

    void RBGUIMaterials::ClearPreviewTextures()
    {
        if(vecUIMaterialDescriptorSet.size() > 0)
        {
            for (VkDescriptorSet descriptorSet : vecUIMaterialDescriptorSet)
            {
                if (descriptorSet != VK_NULL_HANDLE)
                {
                    ImGui_ImplVulkan_RemoveTexture(descriptorSet);
                }
            }
        }
        vecUIMaterialDescriptorSet.clear();
        vecUIMaterialImageViews.clear();
    }

    void RBGUIMaterials::RefreshPreviewTexture(int index)
    {
        if (currentMaterialDescriptors == nullptr || index < 0)
        {
            return;
        }

        const int imageCount = static_cast<int>(currentMaterialDescriptors->rbImageManager.imageBundles.size());
        if (index >= imageCount)
        {
            return;
        }

        if (static_cast<int>(vecUIMaterialDescriptorSet.size()) <= index)
        {
            vecUIMaterialDescriptorSet.resize(index + 1, VK_NULL_HANDLE);
            vecUIMaterialImageViews.resize(index + 1, VK_NULL_HANDLE);
        }

        auto& imageBundle = currentMaterialDescriptors->rbImageManager.imageBundles[index];
        if (vecUIMaterialDescriptorSet[index] != VK_NULL_HANDLE && vecUIMaterialImageViews[index] == imageBundle.imageView)
        {
            return;
        }

        if (vecUIMaterialDescriptorSet[index] != VK_NULL_HANDLE)
        {
            ImGui_ImplVulkan_RemoveTexture(vecUIMaterialDescriptorSet[index]);
        }

        vecUIMaterialDescriptorSet[index] = ImGui_ImplVulkan_AddTexture(imageBundle.sampler, imageBundle.imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        vecUIMaterialImageViews[index] = imageBundle.imageView;
    }

    void RBGUIMaterials::SetMaterial(RBMaterial* material, int descsriptorIndex)
    {
        std::vector<RBDescriptors*>* ptr_Descriptors = RBApplication::GetDescriptors();
        currentMaterialDescriptors = (*ptr_Descriptors)[descsriptorIndex];
        if(currentMaterialDescriptors == nullptr)
        {
            return;
        }
        int imageCount = currentMaterialDescriptors->rbImageManager.imageBundles.size();
#ifdef _WIN32
        ClearPreviewTextures();
#endif
        vecUIMaterialDescriptorSet.resize(imageCount, VK_NULL_HANDLE);
        vecUIMaterialImageViews.resize(imageCount, VK_NULL_HANDLE);
#ifdef _WIN32
        for(int i = 0; i < imageCount; i++)
        {
            RefreshPreviewTexture(i);
        }
#endif
        this->currentMaterial = material;
    }

    void RBGUIMaterials::RefreshPreviewTexture()
    { 
        int imageCount = currentMaterialDescriptors->rbImageManager.imageBundles.size();
        ClearPreviewTextures();
        for(int i = 0; i < imageCount; i++)
        {
            RefreshPreviewTexture(i);
        }
    }

    void RBGUIMaterials::SetLayout()
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 workPos = viewport->WorkPos;
        ImVec2 workSize = viewport->WorkSize;

        // Material Editor: right side panel
        float materialWidth = 360.0f;
        ImGui::SetNextWindowPos(
            ImVec2(workPos.x + workSize.x - materialWidth, workPos.y),
            ImGuiCond_Always
        );
        ImGui::SetNextWindowSize(
            ImVec2(materialWidth, workSize.y),
            ImGuiCond_Always
        );

        ImGui::Begin(
            "Material Editor",
            nullptr,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse
            );
    }
    
    void RBGUIMaterials::Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix)
    {
        if (currentMaterial == nullptr)
        {
            return;
        }

        SetLayout();

        RBMaterialData& material = currentMaterial->GetData();
        
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
        CopyStringToBuffer(shaderPathBuffer, sizeof(shaderPathBuffer), material.shaderDefinationName);
        if (ImGui::InputText("shaderDefinationName", shaderPathBuffer, sizeof(shaderPathBuffer)))
        {
            material.shaderDefinationName = NormalizePathString(shaderPathBuffer);
        }
        bool isChangedShaderRefresh = false;
        if (ImGui::Button("Select Shader..."))
        {
            shaderList = CollectionShaderDefinition();
            ImGui::OpenPopup("Shader Picker");
        }

        if (ImGui::BeginPopupModal("Shader Picker", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (ImGui::Button("Refresh"))
            {
                shaderList = CollectionShaderDefinition();
                isChangedShaderRefresh = true;
            }

            ImGui::Separator();
            ImGui::BeginChild("ShaderList", ImVec2(520, 260), true);

            if (shaderList.empty())
            {
                ImGui::TextDisabled("No shader files found.");
            }
            else
            {
                int index = 0;
                for (const auto& s : shaderList)
                {
                    const bool isSelected = (currentSelectedShader == s);
                    
                     if (ImGui::Selectable(s.c_str(), isSelected))
                    {
                        if(isChangedShaderRefresh)
                        {
                            RBGUIMaterials::SetMaterial(currentMaterial, index);
                        }
                        currentSelectedShader = s;
                        material.shaderDefinition = *RBApplication::GetShaderDefinition(s);
                        std::string shaderReflectionPath = material.shaderDefinition.GetData().stages[(int)RBShaderStageKind::Fragment].path;
                        material.shaderReflection = *RBApplication::GetResourceShader()->GetCustomReflection(shaderReflectionPath);
                        material.shaderDefinationName = NormalizePathString(s);
                        ImGui::CloseCurrentPopup();
                    }
                    index++;
                }
            }

            ImGui::EndChild();

            if (ImGui::Button("Close"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }


        if(material.shaderReflection.descriptorSets.size() > 0)
        {
            auto& descriptorSets = material.shaderReflection.descriptorSets.at(0);
            for(int i = 0; i < descriptorSets.bindings.size(); )
            {
                if(descriptorSets.bindings.find(i + 1) == descriptorSets.bindings.end())
                {
                    i++;
                    continue;
                }

                RBDescriptorBinding& binding = descriptorSets.bindings.find(i + 1)->second;

                if(binding.type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
                {
                    i++;
                    continue;
                }
                float temp = 0.5f;
                const std::string &name = binding.name;
                float value = binding.floatValue;
                if(binding.type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER || binding.type == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE)
                {
                    RefreshPreviewTexture(i);
                    ImVec2 imagePos = ImGui::GetCursorScreenPos();
                    ImGui::Image((ImTextureID)vecUIMaterialDescriptorSet[i], imageVec);
                    
                    // invisible button for drag and drop texture
                    ImGui::SetCursorScreenPos(imagePos);
                    ImGui::InvisibleButton(("drop_target_" + name).c_str(), imageVec);

                    ImDrawList* drawList = ImGui::GetWindowDrawList();
                    bool isHovered = ImGui::IsItemHovered();
                    bool isDraggingOver = false;
                    if (ImGui::BeginDragDropTarget()) 
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_FILE")) 
                        {
                            const char* filePath = (const char*)payload->Data;
                            std::string path(filePath, payload->DataSize);
                            if(path.find("textures") == std::string::npos)
                            {
                                RBLOG_INFO("Dropped file is not a texture: %s", path.c_str());
                                ImGui::Text("Dropped file is not a texture: %s", filePath);
                                ImGui::EndDragDropTarget();
                                continue;
                            }
                            binding.texturePath = NormalizePathString(path);
                            RBLOG_INFO("drag image full path = " + binding.texturePath);
                            if(currentMaterialDescriptors != nullptr)
                            {
                                ImageResourcePtr imageResource = currentMaterialDescriptors->getTextureImage(i);
                                bool alreadyQueued = false;
                                for (const ImageResourcePtr& oldImageResource : RBApplication::oldImageResourceVec)
                                {
                                    if (oldImageResource.imageView == imageResource.imageView)
                                    {
                                        alreadyQueued = true;
                                        break;
                                    }
                                }
                                if (!alreadyQueued)
                                {
                                    RBApplication::oldImageResourceVec.push_back(imageResource);
                                }

                                currentMaterialDescriptors->updateTextureImagePath(i, binding.texturePath);
                                bool alreadyUpdated = false;
                                for (RBDescriptors* updatedDescriptor : *RBApplication::GetUpdatedDescriptors())
                                {
                                    if (updatedDescriptor == currentMaterialDescriptors)
                                    {
                                        alreadyUpdated = true;
                                        break;
                                    }
                                }
                                if (!alreadyUpdated)
                                {
                                    RBApplication::GetUpdatedDescriptors()->push_back(currentMaterialDescriptors);
                                }
                                
                                RBApplication::descriptorSetsUpdate = true;
                                RBLOG_INFO("descriptorSetsUpdate = true");
                            }
                            RBLOG_INFO("Dropped texture: %s", binding.texturePath.c_str());
                            ImGui::Text("Dropped: %s", filePath);
                        }
                        ImGui::EndDragDropTarget();
                        isDraggingOver = ImGui::IsItemHovered();
                    }

                    // draw border around the image based on hover and drag state
                    ImU32 borderColor;
                    if (isDraggingOver) 
                    {
                        borderColor = IM_COL32(0, 255, 0, 255);
                    } else if (isHovered) 
                    {
                        borderColor = IM_COL32(255, 255, 255, 255);
                    } else 
                    {
                        borderColor = IM_COL32(128, 128, 128, 128);
                    }
                    
                    drawList->AddRect(imagePos, ImVec2(imagePos.x + imageVec.x, imagePos.y + imageVec.y), 
                    borderColor, 0, 0, 1.0f);

                    ImGui::SameLine();
                    ImGui::Text(name.c_str());
                    i++;
                }
                else
                {
                    ImGui::SliderFloat(name.c_str(), &value, 0, 1);
                    binding.floatValue = value;
                }
            }
        }

        if (ImGui::Button("Reset"))
        {
            currentMaterial->Reset();
        }
        ImGui::SameLine();
        if (ImGui::Button("Load"))
        {
            currentMaterial->Load(currentMaterial->GetPath() + material.name);
        }
        ImGui::SameLine();
        if (ImGui::Button("Save"))
        {
            currentMaterial->Save(currentMaterial->GetPath() + material.name, material);
        }

        ImGui::TextWrapped("Edit the material properties and save them as .mat files.");
        ImGui::End();
    }
}