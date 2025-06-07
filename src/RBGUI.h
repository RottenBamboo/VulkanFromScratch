//
// Created by rottenbamboo on 2025/6/6.
//

#pragma once

#include "RBDevice.h"
namespace RottenBamboo 
{
    class RBGUI 
    {
    
    public:

        bool checkbox = false;

        VkDescriptorPool imguiDescriptorPool;    

        RBGUI();

        ~RBGUI();

        void Initialize(SDL_Window* window, RBDevice& device, VkRenderPass renderPass);

        void Reinitialize(SDL_Window* window, RBDevice& device, VkRenderPass renderPass);

        void Render(VkCommandBuffer& commandBuffer);

        void createDescriptorPool(RBDevice& device);
    };

}