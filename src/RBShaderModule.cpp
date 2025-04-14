#include "RBShaderModule.h"
#include <stdexcept>
#include <iostream>
namespace RottenBamboo {
    RBShaderModule::RBShaderModule(VkDevice device, const VkShaderModuleCreateInfo &createInfo) : device(device) 
    {
        if (vkCreateShaderModule(device, &createInfo, nullptr, &module) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }
        std::cout << "RBShaderModule::RBShaderModule()" << std::endl;
    }

    RBShaderModule::~RBShaderModule() 
    {
        vkDestroyShaderModule(device, module, nullptr);
        std::cout << "RBShaderModule::~RBShaderModule()" << std::endl;
    }

    VkShaderModule RBShaderModule::get() const 
    {
        return module;
        std::cout << "RBShaderModule::get()" << std::endl;
    }
}