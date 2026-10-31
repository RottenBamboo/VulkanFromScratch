//
// Created by rottenbamboo on 2026/10/29.
//

#pragma once

#include "RBGUIBase.h"
#include <iostream>
namespace RottenBamboo 
{
    class RBGUIFileMenu : public RBGUIBase 
    {
    public:
        RBGUIFileMenu(RBDevice& device);

        void DrawContent();
        virtual ~RBGUIFileMenu() override = default;
        
        virtual void Initialize(VkRenderPass renderPass) override;

        virtual void Render(VkCommandBuffer& commandBuffer, UniformBufferShaderVariables& uniformMatrix) override;
        
        void RenderFileToolbar(VkCommandBuffer& commandBuffer);  

        void SetLayout() override;
    private:
        RBDevice& rbDevice;
        bool m_ShouldClose = false;
        bool m_OpenSceneBrowser = false;
        std::vector<std::string> m_SceneFiles;

        void RefreshSceneFiles();
        void DrawOpenSceneBrowser();
    };
}