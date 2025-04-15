#pragma once
#include <vulkan/vulkan_core.h>
#include "RBDevice.h"

namespace RottenBamboo {
    class RBShaderModule 
    {
        RBDevice &device;
        VkShaderModule module;
        VkShaderModuleCreateInfo createInfo;

    public:
        RBShaderModule(RBDevice &device);
        ~RBShaderModule();
        void fillCreateInfo(const std::vector<char> &code);
        void createShaderModule(RBDevice &device, const std::vector<char> &code);

        VkShaderModule get() const;
    };
}