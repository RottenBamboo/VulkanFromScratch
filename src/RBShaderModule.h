#include <vulkan/vulkan_core.h>

namespace RottenBamboo {
    class RBShaderModule 
    {
        VkDevice device;
        VkShaderModule module;

    public:
        RBShaderModule(VkDevice device, const VkShaderModuleCreateInfo &createInfo);
        ~RBShaderModule();
        VkShaderModule get() const;
    };
}