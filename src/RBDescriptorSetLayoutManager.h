//
// Created by RottenBamboo on 2023/7/26.
//

#ifndef VULKANFROMSCRATCH_RBDESCRIPTORSETLAYOUTMANAGER_H
#define VULKANFROMSCRATCH_RBDESCRIPTORSETLAYOUTMANAGER_H

#include <vulkan/vulkan.h>
#include "RBCommon.h"
#include "RBDevice.h"

namespace RottenBamboo {

    class RBDescriptorSetLayoutManager {

    private:
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        RBDevice &rbDevice;
        VkDescriptorSetLayout descriptorSetLayout;

    public:
        RBDescriptorSetLayoutManager(RBDevice &device);
        void fillDescriptorSetLayoutBinding(RBDevice &device, std::vector<VkDescriptorSetLayoutBinding> bindings);
        void fillDescriptorByType(VkDescriptorSetLayoutBinding bindings);
        ~RBDescriptorSetLayoutManager();
        VkDescriptorSetLayout createDescriptorSetLayout();
        void fillDescriptorSetSamplerLayout();
        void fillDescriptorSetUboLayout();
    };

}


#endif //VULKANFROMSCRATCH_RBDESCRIPTORSETLAYOUTMANAGER_H
