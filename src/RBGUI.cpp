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
        checkbox = false;
        std::cout << "RBGUI::RBGUI()" << std::endl;
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
        imguiDescriptorPool = VK_NULL_HANDLE;
    }

    RBGUI::~RBGUI() 
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        std::cout << "RBGUI::~RBGUI()" << std::endl;
    }

    void RBGUI::createDescriptorPool(RBDevice& device)
    {
        if (imguiDescriptorPool != VK_NULL_HANDLE) {
            vkDestroyDescriptorPool(device.device, imguiDescriptorPool, nullptr);
            imguiDescriptorPool = VK_NULL_HANDLE;
        }
        VkDescriptorPoolSize poolSizes[] = 
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 100 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 100 },
        };

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1000;
        poolInfo.poolSizeCount = static_cast<uint32_t>(std::size(poolSizes));
        poolInfo.pPoolSizes = poolSizes;

        vkCreateDescriptorPool(device.device, &poolInfo, nullptr, &imguiDescriptorPool);
    }

    void RBGUI::Initialize(SDL_Window* window, RBDevice& device, VkRenderPass renderPass)
    {
        createDescriptorPool(device);

        ImGui_ImplSDL3_InitForVulkan(window);

        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = device.instance;
        init_info.PhysicalDevice = device.physicalDevice;
        init_info.Device = device.device;
        init_info.QueueFamily = device.findQueueFamilies(device.physicalDevice).graphicsFamily.value();
        init_info.Queue = device.graphicsQueue;
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = imguiDescriptorPool;
        init_info.MinImageCount = MAX_FRAMES_IN_FLIGHT;
        init_info.ImageCount = MAX_FRAMES_IN_FLIGHT;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.RenderPass = renderPass;
        init_info.Subpass = 0;

        ImGui_ImplVulkan_Init(&init_info);
    }

    void RBGUI::Render(VkCommandBuffer& commandBuffer)
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowSizeConstraints(ImVec2(200, 100), ImVec2(600, 1000));
        ImGui::Begin("Example Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        //ImGui::Begin("Example Window");
        ImGui::Checkbox("Checkbox!", &checkbox);
        ImGui::Text("IMGUI Test");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
    }
}
// ...existing code...