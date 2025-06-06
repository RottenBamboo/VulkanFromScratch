//
// Created by rottenbamboo on 2025/6/6.
//
#include "RBCommandBuffer.h"
namespace RottenBamboo 
{
    class RBGUI 
    {
    public:
        RBGUI();

        ~RBGUI();

        void Initialize(SDL_Window* window, RBDevice& device, VkRenderPass renderPass, VkDescriptorPool descriptorPool);

        void Render(VkCommandBuffer& commandBuffer);
    };

}