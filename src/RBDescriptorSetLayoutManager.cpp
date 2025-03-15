//
// Created by RottenBamboo on 2023/7/26.
//

#include "RBDescriptorSetLayoutManager.h"

namespace RottenBamboo {
    RBDescriptorSetLayoutManager::RBDescriptorSetLayoutManager(RBDevice &device) : rbDevice(device)
    {
    }

    void RBDescriptorSetLayoutManager::fillDescriptorSetLayoutBinding(RBDevice &device, std::vector<VkDescriptorSetLayoutBinding> bindings)
    {
        for(int i = 0; i < bindings.size(); i++)
        {
            fillDescriptorByType(bindings[i]);
        }
    }
    RBDescriptorSetLayoutManager::~RBDescriptorSetLayoutManager()
    {
        Destroy();
        std::cout << "RBDescriptorSetLayoutManager::~RBDescriptorSetLayoutManager()" << std::endl;
    }

    void RBDescriptorSetLayoutManager::Destroy()
    {
        vkDestroyDescriptorSetLayout(rbDevice.device, descriptorSetLayout, nullptr);
        std::cout << "RBDescriptorSetLayoutManager::Destroy()" << std::endl;
    }

    void RBDescriptorSetLayoutManager::fillDescriptorByType(VkDescriptorSetLayoutBinding binding)
    {
        VkDescriptorSetLayoutBinding bindingVarable{};
        bindingVarable = binding;
        bindingVarable.pImmutableSamplers = nullptr;
        this->bindings.push_back(bindingVarable);
    }

    void RBDescriptorSetLayoutManager::createDescriptorSetLayout() {
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(rbDevice.device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }
}