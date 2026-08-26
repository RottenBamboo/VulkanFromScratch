//
// Created by rottenbamboo on 2025/4/13.
//
#include "RBShaderModule.h"
#include <stdexcept>
#include <iostream>
namespace RottenBamboo {
    RBShaderModule::RBShaderModule(RBDevice &device) : device(device) 
    {
        std::cout << "RBShaderModule::RBShaderModule()" << std::endl;
        RBLOG_INFO("RBShaderModule::RBShaderModule()");
    }

    RBShaderModule::~RBShaderModule() 
    {
        Destroy();
        std::cout << "RBShaderModule::~RBShaderModule()" << std::endl;
        RBLOG_INFO("RBShaderModule::~RBShaderModule()");
    }

    void RBShaderModule::Destroy()
    {
        if (module != VK_NULL_HANDLE)
        {
            vkDestroyShaderModule(device.device, module, nullptr);
            module = VK_NULL_HANDLE;
        }
        std::cout << "RBShaderModule::Destroy()" << std::endl;
        RBLOG_INFO("RBShaderModule::Destroy()");
    }
    void RBShaderModule::fillCreateInfo(const std::vector<char> &code)
    {
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.flags = 0;
        createInfo.pNext = nullptr;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());
    }

    void RBShaderModule::createShaderModule(RBDevice &device, const std::vector<char> &code)
    {
        if (module != VK_NULL_HANDLE)
        {
            vkDestroyShaderModule(device.device, module, nullptr);
            module = VK_NULL_HANDLE;
        }

        fillCreateInfo(code);

        if (vkCreateShaderModule(device.device, &createInfo, nullptr, &module) != VK_SUCCESS) {
            RBLOG_FATAL("Failed to create shader module!");
            throw std::runtime_error("failed to create shader module!");
        }
        std::cout << "RBShaderModule::createShaderModule()" << std::endl;
        RBLOG_INFO("RBShaderModule::createShaderModule()");
    }
    VkShaderModule RBShaderModule::get() const
    {
        std::cout << "RBShaderModule::get()" << std::endl;
        RBLOG_INFO("RBShaderModule::get()");
        return module;
    }
}