//
// Created by RottenBamboo on 2023/7/26.
//

#pragma once

#ifndef VULKANFROMSCRATCH_RBDESCRIPTORSETLAYOUTMANAGER_H
#define VULKANFROMSCRATCH_RBDESCRIPTORSETLAYOUTMANAGER_H

#include <vulkan/vulkan.h>
#include "RBCommon.h"
#include "RBDevice.h"

namespace RottenBamboo {

    class RBDescriptorSetLayoutManager {

    public:
        VkDescriptorSetLayout descriptorSetLayout;
    private:
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        RBDevice &rbDevice;

    public:
        RBDescriptorSetLayoutManager(RBDevice &device);
        void fillDescriptorSetLayoutBinding(RBDevice &device, std::vector<VkDescriptorSetLayoutBinding> bindings);
        void fillDescriptorByType(VkDescriptorSetLayoutBinding bindings);
        ~RBDescriptorSetLayoutManager();
        void createDescriptorSetLayout();
        void Destroy();
    };

}


#endif //VULKANFROMSCRATCH_RBDESCRIPTORSETLAYOUTMANAGER_H
