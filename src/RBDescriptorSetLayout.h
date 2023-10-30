//
// Created by RottenBamboo on 2023/7/26.
//

#ifndef VULKANFROMSCRATCH_RBDESCRIPTORSETLAYOUT_H
#define VULKANFROMSCRATCH_RBDESCRIPTORSETLAYOUT_H

#include <vulkan/vulkan.h>
#include "RBCommon.h"
#include "RBDevice.h"

namespace RottenBamboo {

    class RBDescriptorSetLayout {

    private:
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        RBDevice &rbDevice;
        VkDescriptorSetLayout descriptorSetLayout;

    public:
        RBDescriptorSetLayout(RBDevice &device);
        ~RBDescriptorSetLayout();
        void createDescriptorSetLayout();
        void fillDescriptorSetSamplerLayout();
        void fillDescriptorSetUboLayout();
    };

}


#endif //VULKANFROMSCRATCH_RBDESCRIPTORSETLAYOUT_H
