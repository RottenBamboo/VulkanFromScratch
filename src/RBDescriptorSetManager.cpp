//
// Created by RottenBamboo on 2023/7/26.
//
//
#include "RBDescriptorSetManager.h"
namespace RottenBamboo
{
    RBDescriptorSetManager::RBDescriptorSetManager(RBDevice &device) : rbDevice(device)
    {
        std::cout << "RBDescriptorSetManager::RBDescriptorSetManager()" << std::endl;
            RBLOG_INFO("RBDescriptorSetManager::RBDescriptorSetManager()");
    }

    RBDescriptorSetManager::~RBDescriptorSetManager()
    {
        Destroy();
        std::cout << "RBDescriptorSetManager::~RBDescriptorSetManager()" << std::endl;
            RBLOG_INFO("RBDescriptorSetManager::~RBDescriptorSetManager()");
    }

    void RBDescriptorSetManager::Destroy()
    {
        clearDescriptorSets();
        clearDescriptorWrites();
        descriptorPoolManager.Destroy();
        descriptorSetLayoutManager.Destroy();
        descriptorSets.clear();
        descriptorWrites.clear();
        std::cout << "RBDescriptorSetManager::Destroy()" << std::endl;
            RBLOG_INFO("RBDescriptorSetManager::Destroy()");
    }

    void RBDescriptorSetManager::fillDescriptorSetsAllocateInfo(VkDescriptorPool &descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSetLayout* pSetLayouts)
    {
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.pNext = nullptr;
        allocInfo.descriptorSetCount = descriptorSetCount;
        allocInfo.pSetLayouts = pSetLayouts;
        std::cout << "RBDescriptorSetManager::fillDescriptorSetsAllocateInfo()" << std::endl;
            RBLOG_INFO("RBDescriptorSetManager::fillDescriptorSetsAllocateInfo()");
    }

    void RBDescriptorSetManager::fillDescriptotSetsWriteBuffer(uint32_t dstSetIndex, uint32_t dstBinding, uint32_t dstArrayElement, uint32_t descriptorCount, VkDescriptorType descriptorType, const VkDescriptorBufferInfo* pBufferInfo)
    {
        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[dstSetIndex];
        descriptorWrite.dstBinding = dstBinding;
        descriptorWrite.dstArrayElement = dstArrayElement;
        descriptorWrite.descriptorCount = descriptorCount;
        descriptorWrite.descriptorType = descriptorType;
        descriptorWrite.pBufferInfo = pBufferInfo;
        this->descriptorWrites.push_back(descriptorWrite);
        //std::cout << "RBDescriptorSetManager::fillDescriptotSetsWriteBuffer()" << std::endl;
    }

    void RBDescriptorSetManager::fillDescriptotSetsWriteImage(uint32_t dstSetIndex, uint32_t dstBinding, uint32_t dstArrayElement, uint32_t descriptorCount, VkDescriptorType descriptorType, const VkDescriptorImageInfo* pImageInfo)
    {
        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[dstSetIndex];
        descriptorWrite.dstBinding = dstBinding;
        descriptorWrite.dstArrayElement = dstArrayElement;
        descriptorWrite.descriptorCount = descriptorCount;
        descriptorWrite.descriptorType = descriptorType;
        descriptorWrite.pImageInfo = pImageInfo;
        this->descriptorWrites.push_back(descriptorWrite);
        //std::cout << "RBDescriptorSetManager::fillDescriptotSetsWriteImage()" << std::endl;
    }

    void RBDescriptorSetManager::allocateDescriptorSets(RBDevice &device, int size)
    {
        descriptorSets.resize(size);
        if(vkAllocateDescriptorSets(rbDevice.device, &(this->allocInfo), descriptorSets.data()) != VK_SUCCESS)
        {
            RBLOG_FATAL("failed to allocate descriptor sets!");
            throw std::runtime_error("failed to allocate descriptor sets!");
        }
        std::cout << "RBDescriptorSetManager::allocateDescriptorSets()" << std::endl;
            RBLOG_INFO("RBDescriptorSetManager::allocateDescriptorSets()");
    }

    void RBDescriptorSetManager::updateDescriptorSets(RBDevice &device)
    {
        vkUpdateDescriptorSets(device.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        clearDescriptorWrites();
        //std::cout << "RBDescriptorSetManager::updateDescriptorSets()" << std::endl;
    }

    void RBDescriptorSetManager::updateDescriptorSets(RBDevice &device, std::vector<VkWriteDescriptorSet> descriptorWrites)
    {
        vkUpdateDescriptorSets(device.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        //std::cout << "RBDescriptorSetManager::updateDescriptorSets(RBDevice &device, std::vector<VkWriteDescriptorSet> descriptorWrites)" << std::endl;
    }

    void RBDescriptorSetManager::clearDescriptorWrites()
    {
        descriptorWrites.clear();
        descriptorWrites.reserve(0);
        //std::cout << "RBDescriptorSetManager::clearDescriptorWrites() " << "descriptorWrites size: " << descriptorWrites.size() << std::endl;
    }

    void RBDescriptorSetManager::clearDescriptorSets()
    {
        descriptorSets.clear();
        descriptorSets.reserve(0);
        std::cout << "RBDescriptorSetManager::clearDescriptorSets() " << "descriptorWrites size: " << descriptorWrites.size() << std::endl;
            RBLOG_INFO("RBDescriptorSetManager::clearDescriptorSets() " "descriptorWrites size: %d", descriptorWrites.size());
    }

    VkDescriptorSet* RBDescriptorSetManager::getDescriptorSet(int index)
    {
        return &descriptorSets[index];
    }

}