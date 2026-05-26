#include "RBGUIAssets.h"
#include "RBApplication.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <imgui.h>
#include <sstream>

namespace RottenBamboo
{
    namespace
    {
        std::string ToLowerCopy(std::string value)
        {
            std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
                return static_cast<char>(std::tolower(c));
            });
            return value;
        }

        bool ContainsCaseInsensitive(const std::string& value, const std::string& query)
        {
            if (query.empty()) {
                return true;
            }
            return ToLowerCopy(value).find(ToLowerCopy(query)) != std::string::npos;
        }

        bool IsMaterialFile(const std::filesystem::path& path)
        {
           const std::string ext = ToLowerCopy(path.extension().string());
           return ext == ".mat" || ext == ".material";
        }
    }

    RBGUIAssets::RBGUIAssets()
        : resourceManager(nullptr), currentDirectory(GET_PROJECT_ROOT_DIR)
    {
    }

    void RBGUIAssets::SetResourceManager(ResourceManager* resourceManager)
    {
        this->resourceManager = resourceManager;
    }

    void RBGUIAssets::Initialize(VkRenderPass renderPass)
    {
        (void)renderPass;
    }

    bool RBGUIAssets::IsModelFile(const std::filesystem::path& path)
    {
        const std::string ext = ToLowerCopy(path.extension().string());
        return ext == ".obj" || ext == ".fbx" || ext == ".gltf" || ext == ".glb" || ext == ".dae";
    }

    std::string RBGUIAssets::HumanReadableSize(std::uintmax_t bytes)
    {
        constexpr double kb = 1024.0;
        constexpr double mb = kb * 1024.0;
        constexpr double gb = mb * 1024.0;

        std::ostringstream stream;
        stream << std::fixed << std::setprecision(2);
        if (bytes >= static_cast<std::uintmax_t>(gb)) {
            stream << (static_cast<double>(bytes) / gb) << " GB";
        }
        else if (bytes >= static_cast<std::uintmax_t>(mb)) {
            stream << (static_cast<double>(bytes) / mb) << " MB";
        }
        else if (bytes >= static_cast<std::uintmax_t>(kb)) {
            stream << (static_cast<double>(bytes) / kb) << " KB";
        }
        else {
            stream << bytes << " B";
        }
        return stream.str();
    }

    void RBGUIAssets::NavigateTo(const std::filesystem::path& path)
    {
        std::error_code ec;
        if (std::filesystem::exists(path, ec) && std::filesystem::is_directory(path, ec)) {
            currentDirectory = path;
        }
    }

    void RBGUIAssets::RenderQuickAccess()
    {
        ImGui::TextUnformatted("Quick Access");

        const std::filesystem::path root = GET_PROJECT_ROOT_DIR;
        if (ImGui::Button("Project Root")) {
            NavigateTo(root);
        }
        ImGui::SameLine();
        if (ImGui::Button("Models")) {
            NavigateTo(root / "models");
        }
        ImGui::SameLine();
        if (ImGui::Button("Materials")) {
            NavigateTo(root / "materials");
        }
        ImGui::SameLine();
        if (ImGui::Button("Textures")) {
            NavigateTo(root / "textures");
        }
        ImGui::SameLine();
        if (ImGui::Button("Shader")) {
            NavigateTo(root / "shader");
        }

        ImGui::Separator();
    }

    void RBGUIAssets::RenderDirectoryContents()
    {
        ImGui::Text("Directory: %s", currentDirectory.string().c_str());

        if (ImGui::Button("Up One Level")) {
            NavigateTo(currentDirectory.parent_path());
        }

        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1.0f);
        ImGui::InputTextWithHint("##AssetSearch", "Search file or folder", searchText.data(), searchText.size());

        ImGui::Separator();

        if (!std::filesystem::exists(currentDirectory)) {
            ImGui::TextUnformatted("Current directory does not exist.");
            return;
        }

        std::error_code ec;
        if (ImGui::BeginChild("AssetDirectoryList", ImVec2(0, 320), true)) {
            std::vector<std::filesystem::directory_entry> directories;
            std::vector<std::filesystem::directory_entry> files;

            for (const auto& entry : std::filesystem::directory_iterator(currentDirectory, ec)) {
                if (searchText[0] != '\0' && !ContainsCaseInsensitive(entry.path().filename().string(), searchText.data())) {
                    continue;
                }

                if (entry.is_directory(ec)) {
                    directories.push_back(entry);
                }
                else {
                    files.push_back(entry);
                }
            }

            std::sort(directories.begin(), directories.end(), [](const auto& lhs, const auto& rhs) {
                return lhs.path().filename().string() < rhs.path().filename().string();
            });
            std::sort(files.begin(), files.end(), [](const auto& lhs, const auto& rhs) {
                return lhs.path().filename().string() < rhs.path().filename().string();
            });

            ImGui::TextUnformatted("Folders");
            for (const auto& entry : directories) {
                const bool selected = selectedPath == entry.path();
                const std::string label = std::string("Folder: ") + entry.path().filename().string();
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 200, 80, 255));
                if (ImGui::Selectable(label.c_str(), selected)) {
                    selectedPath = entry.path();
                }
                ImGui::PopStyleColor();
                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    NavigateTo(entry.path());
                }
            }

            ImGui::Separator();
            ImGui::TextUnformatted("Files");
            for (const auto& entry : files) {
                const bool selected = selectedPath == entry.path();
                const std::string label = std::string("File: ") + entry.path().filename().string();
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(200, 200, 200, 255));
                if (ImGui::Selectable(label.c_str(), selected)) {
                    selectedPath = entry.path();
                }

                const bool doubleClicked =
                    ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);

                ImGui::PopStyleColor();

                 if (doubleClicked) {
                     selectedPath = entry.path();

                    if (IsMaterialFile(entry.path()) && resourceManager != nullptr) {
                        const std::string path = entry.path().string();
                        RBApplication::GetGUI()->SetEditorMaterial(resourceManager->Load<RBMaterial>(path).get());
                    }
                }
            }
        }
        ImGui::EndChild();
    }

    void RBGUIAssets::RenderLoadedModels()
    {
        if (resourceManager == nullptr) {
            ImGui::TextUnformatted("Resource manager is not connected.");
            return;
        }

        const auto modelPaths = resourceManager->GetPaths<RBModel>();
        ImGui::Text("Loaded Models (%zu)", modelPaths.size());

        if (ImGui::BeginChild("LoadedModelList", ImVec2(0, 220), true)) {
            for (const auto& path : modelPaths) {
                auto model = resourceManager->Get<RBModel>(path);
                const bool selected = selectedPath == std::filesystem::path(path);

                if (ImGui::Selectable(path.c_str(), selected)) {
                    selectedPath = std::filesystem::path(path);
                }

                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    selectedPath = std::filesystem::path(path);
                }

                ImGui::SameLine();
                const std::string meshInfo = model ? (std::to_string(model->MeshCount()) + " meshes") : "unavailable";
                ImGui::TextDisabled("%s", meshInfo.c_str());
            }
        }
        ImGui::EndChild();
    }

    void RBGUIAssets::RenderAssetDetails()
    {
        ImGui::TextUnformatted("Details");
        ImGui::Separator();

        if (selectedPath.empty()) {
            ImGui::TextUnformatted("Select a file or loaded model.");
            return;
        }

        ImGui::Text("Path: %s", selectedPath.string().c_str());
        ImGui::Text("Name: %s", selectedPath.filename().string().c_str());
        ImGui::Text("Extension: %s", selectedPath.extension().string().c_str());

        std::error_code ec;
        if (std::filesystem::exists(selectedPath, ec) && std::filesystem::is_regular_file(selectedPath, ec)) {
            ImGui::Text("File Size: %s", HumanReadableSize(std::filesystem::file_size(selectedPath, ec)).c_str());
            ImGui::Text("Source: file system");
        }

        if (resourceManager != nullptr && resourceManager->IsLoaded<RBModel>(selectedPath.string())) {
            auto model = resourceManager->Get<RBModel>(selectedPath.string());
            if (model) {
                ImGui::Text("Loaded as model: yes");
                ImGui::Text("Mesh Count: %zu", model->MeshCount());

                if (ImGui::Button("Reload Model")) {
                    resourceManager->Reload<RBModel>(selectedPath.string());
                }
                ImGui::SameLine();
                if (ImGui::Button("Unload Model")) {
                    resourceManager->Remove<RBModel>(selectedPath.string());
                }
                return;
            }
        }

        ImGui::TextUnformatted("Loaded as model: no");
        if (resourceManager != nullptr && IsModelFile(selectedPath)) {
            if (ImGui::Button("Load as Model")) {
                resourceManager->Load<RBModel>(selectedPath.string());
            }
        }
        
        // if (resourceManager != nullptr && resourceManager->IsLoaded<RBMaterial>(selectedPath.string())) {
        //     auto material = resourceManager->Get<RBMaterial>(selectedPath.string());
        //     if (material) {
        //         ImGui::TextUnformatted("Loaded as material: yes");

        //         if (ImGui::Button("Reload Material")) {
        //             resourceManager->Reload<RBMaterial>(selectedPath.string());
        //         }
        //         ImGui::SameLine();
        //         if (ImGui::Button("Unload Material")) {
        //             resourceManager->Remove<RBMaterial>(selectedPath.string());
        //         }
        //         return;
        //     }
        // }

        // if (resourceManager != nullptr && IsMaterialFile(selectedPath)) {
        //     if (ImGui::Button("Load as Material")) {
        //         resourceManager->Load<RBMaterial>(selectedPath.string());
        //     }
        // }
    }

    void RBGUIAssets::Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix)
    {
        (void)commandBuffer;
        (void)uniformMatrix;

        ImGui::Begin("Asset Manager", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        RenderQuickAccess();
        RenderDirectoryContents();
        RenderLoadedModels();
        RenderAssetDetails();

        ImGui::End();
    }
}