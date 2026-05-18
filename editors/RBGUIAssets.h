//
// Created by rottenbamboo on 2026/5/19.
//

#pragma once

#include "RBGUIBase.h"
#include "../src/RBResourceManager.h"
#include <filesystem>
#include <array>

namespace RottenBamboo 
{
    class RBGUIAssets : public RBGUIBase 
    {
    public:
        RBGUIAssets();
        virtual ~RBGUIAssets() override = default;

        void Initialize(VkRenderPass renderPass) override;
        void Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix) override;

        void SetResourceManager(ResourceManager* resourceManager);

    private:
        void RenderQuickAccess();
        void RenderDirectoryContents();
        void RenderLoadedModels();
        void RenderAssetDetails();
        void NavigateTo(const std::filesystem::path& path);
        static bool IsModelFile(const std::filesystem::path& path);
        static std::string HumanReadableSize(std::uintmax_t bytes);

    private:
        ResourceManager* resourceManager;
        std::filesystem::path currentDirectory;
        std::filesystem::path selectedPath;
        std::array<char, 256> searchText{};
    };
}