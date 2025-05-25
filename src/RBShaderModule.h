#pragma once
#include <vulkan/vulkan_core.h>
#include "RBDevice.h"

namespace RottenBamboo {
    class RBShaderModule 
    {
        RBDevice &device;
        VkShaderModuleCreateInfo createInfo;

    public:
    
        VkShaderModule module;
        RBShaderModule(RBDevice &device);
        ~RBShaderModule();
        void Destroy();
        void fillCreateInfo(const std::vector<char> &code);
        void createShaderModule(RBDevice &device, const std::vector<char> &code);

        VkShaderModule get() const;
    };
}