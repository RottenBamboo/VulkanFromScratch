//
// Created by rottenbamboo on 2025/6/6.
//


#include "RBGUI.h"
#include <iostream>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_vulkan.h>

namespace RottenBamboo 
{
    RBGUI::RBGUI() 
    {
        // Constructor implementation
        std::cout << "RBGUI::RBGUI()" << std::endl;
    }

    RBGUI::~RBGUI() 
    {
        // Destructor implementation
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        std::cout << "RBGUI::~RBGUI()" << std::endl;
    }

    void RBGUI::Initialize(SDL_Window* window, RBDevice& device, VkRenderPass renderPass, VkDescriptorPool descriptorPool)
    {
        // this->window = window;
        // this->device = &device;
        // this->renderPass = renderPass;
        // this->descriptorPool = descriptorPool;

        // IMGUI_CHECKVERSION();
        // ImGui::CreateContext();
        // ImGuiIO& io = ImGui::GetIO();
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        // ImGui::StyleColorsDark();

        // ImGui_ImplSDL3_InitForVulkan(window);

        // ImGui_ImplVulkan_InitInfo init_info = {};
        // init_info.Instance = device.instance;
        // init_info.PhysicalDevice = device.physicalDevice;
        // init_info.Device = device.device;
        // init_info.QueueFamily = device.findQueueFamilies(device.physicalDevice).graphicsFamily.value();
        // init_info.Queue = device.graphicsQueue;
        // init_info.PipelineCache = VK_NULL_HANDLE;
        // init_info.DescriptorPool = descriptorPool;
        // init_info.MinImageCount = 2;
        // init_info.ImageCount = 2;
        // init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        // ImGui_ImplVulkan_Init(&init_info, renderPass);

        // // Upload Fonts
        // VkCommandBuffer commandBuffer = ...; // You should provide single-use command buffer here
        // ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        // // submit and wait
        // ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    void RBGUI::Render(VkCommandBuffer& commandBuffer)
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Example Window");
        ImGui::Text("Hello from ImGui!");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
    }
}