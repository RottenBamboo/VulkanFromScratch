#include "RBShaderModule.h"
#include <stdexcept>
#include <iostream>
namespace RottenBamboo {
    RBShaderModule::RBShaderModule(RBDevice &device) : device(device) 
    {
        std::cout << "RBShaderModule::RBShaderModule()" << std::endl;
    }

    RBShaderModule::~RBShaderModule() 
    {
        Destroy();
        std::cout << "RBShaderModule::~RBShaderModule()" << std::endl;
    }

    void RBShaderModule::Destroy()
    {
        vkDestroyShaderModule(device.device, module, nullptr);
        module = VK_NULL_HANDLE;;
        std::cout << "RBShaderModule::Destroy()" << std::endl;
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
        fillCreateInfo(code);

        if (vkCreateShaderModule(device.device, &createInfo, nullptr, &module) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }
        std::cout << "RBShaderModule::createShaderModule()" << std::endl;
    }
    VkShaderModule RBShaderModule::get() const
    {
        std::cout << "RBShaderModule::get()" << std::endl;
        return module;
    }
}