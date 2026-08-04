//
// Created by rottenbamboo on 2023/5/26.
//

#include "RBSwapChain.h"
#include "RBCommon.h"

namespace RottenBamboo{

    QueueFamilyIndices RBSwapChain::findQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR* surface)
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, *surface, &presentSupport);

            if (presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.isComplete()) {
                break;
            }

            i++;
        }
        return indices;
    }

    void RBSwapChain::SetDepthView(VkImageView* depthView)
    {
        //depthImageView = depthView;
    }

    void RBSwapChain::createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits  numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = mipLevels;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = numSamples;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(refDevice.device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
            RBLOG_FATAL("Failed to create image!");
            throw std::runtime_error("failed to create image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(refDevice.device, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(refDevice.physicalDevice, memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(refDevice.device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
            RBLOG_FATAL("Failed to allocate image memory!");
            throw std::runtime_error("failed to allocate image memory!");
        }

        vkBindImageMemory(refDevice.device, image, imageMemory, 0);
    }

    VkImageView RBSwapChain::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;
        viewInfo.subresourceRange.aspectMask = aspectFlags;

        VkImageView imageView;
        if(vkCreateImageView(refDevice.device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
        {
            RBLOG_FATAL("Failed to create image view!");
            throw std::runtime_error("failed to create texture image view!");
        }
        return imageView;
    }


    void RBSwapChain::createColorResources()
    {
        VkFormat colorFormat = swapChainImageFormat;
        createImage(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
        colorImageView = createImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

        RBLOG_INFO("RBSwapChain::createColorResources() - Created MSAA color image (samples: {}) and resolve image (samples: 1)", msaaSamples);
    }

    VkFormat RBSwapChain::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
    {
        for(VkFormat format : candidates)
        {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(refDevice.physicalDevice, format, &props);

            if(tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
            {
                return format;
            }
            else if(tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
            {
                return format;
            }
        }

        RBLOG_FATAL("Failed to find supported format!");
        throw std::runtime_error("failed to find supported format!");

    }

    VkFormat RBSwapChain::findDepthFormat()
    {
        return findSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

    void RBSwapChain::createDepthResources()
    {
        VkFormat depthFormat = findDepthFormat();
        createImage(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
        depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
        RBLOG_INFO("RBSwapChain::createDepthResources()");
    }

    void RBSwapChain::createRenderPass()
    {
        const bool useResolveAttachment = (msaaSamples != VK_SAMPLE_COUNT_1_BIT);

        // Helper: build a single-color (no depth) render pass for Sky/Lighting passes.
        // Sky pass clears (UNDEFINED → COLOR_ATTACHMENT_OPTIMAL);
        // Lighting pass loads sky output (COLOR_ATTACHMENT_OPTIMAL → PRESENT_SRC_KHR).
        auto makeColorOnlyPass = [&](VkAttachmentLoadOp colorLoadOp,
                                     VkImageLayout     colorInitialLayout,
                                     VkImageLayout     colorFinalLayout,
                                     VkRenderPass&     outPass)
        {
            VkAttachmentDescription colorAttachment{};
            colorAttachment.format         = swapChainImageFormat;
            colorAttachment.samples        = msaaSamples;
            colorAttachment.loadOp         = colorLoadOp;
            colorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachment.initialLayout  = colorInitialLayout;
            colorAttachment.finalLayout    = useResolveAttachment
                                             ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                                             : colorFinalLayout;

            VkAttachmentDescription colorResolve{};
            colorResolve.format         = swapChainImageFormat;
            colorResolve.samples        = VK_SAMPLE_COUNT_1_BIT;
            colorResolve.loadOp         = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorResolve.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
            colorResolve.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorResolve.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
            colorResolve.finalLayout    = colorFinalLayout;

            VkAttachmentReference colorRef{};
            colorRef.attachment = 0;
            colorRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkAttachmentReference resolveRef{};
            resolveRef.attachment = 1;
            resolveRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkSubpassDescription subpass{};
            subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass.colorAttachmentCount = 1;
            subpass.pColorAttachments    = &colorRef;
            subpass.pDepthStencilAttachment = nullptr;
            subpass.pResolveAttachments  = useResolveAttachment ? &resolveRef : nullptr;

            VkSubpassDependency dependency{};
            dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
            dependency.dstSubpass    = 0;
            dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            dependency.srcAccessMask = 0;
            dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            std::vector<VkAttachmentDescription> attachments;
            attachments.push_back(colorAttachment);
            if (useResolveAttachment)
                attachments.push_back(colorResolve);

            VkRenderPassCreateInfo rpInfo{};
            rpInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            rpInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            rpInfo.pAttachments    = attachments.data();
            rpInfo.subpassCount    = 1;
            rpInfo.pSubpasses      = &subpass;
            rpInfo.dependencyCount = 1;
            rpInfo.pDependencies   = &dependency;

            if (vkCreateRenderPass(refDevice.device, &rpInfo, nullptr, &outPass) != VK_SUCCESS)
            {
                RBLOG_FATAL("failed to create render pass!");
                throw std::runtime_error("failed to create render pass!");
            }
        };

        // Sky pass: clears the swap-chain image at the start of each frame.
        makeColorOnlyPass(VK_ATTACHMENT_LOAD_OP_CLEAR,
                          VK_IMAGE_LAYOUT_UNDEFINED,
                          VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                          renderPassSky);

        // Lighting pass: loads the sky output, then transitions to present.
        makeColorOnlyPass(VK_ATTACHMENT_LOAD_OP_LOAD,
                          VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                          VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                          renderPass);

        RBLOG_INFO("RBSwapChain::createRenderPass()");
    }

    void RBSwapChain::createFrameBuffers()
    {
        const bool useResolveAttachment = (msaaSamples != VK_SAMPLE_COUNT_1_BIT);

        swapChainFrameBuffers.resize(swapChainImageViews.size());
        for(size_t i = 0; i < swapChainImageViews.size(); i++)
        {
            // No depth attachment in the Sky/Lighting render passes.
            std::vector<VkImageView> attachments{};
            if (useResolveAttachment)
            {
                // MSAA: attachment 0 = MSAA color, attachment 1 = resolve (swapchain)
                attachments.push_back(colorImageView);
                attachments.push_back(swapChainImageViews[i]);
            }
            else
            {
                // Non-MSAA: attachment 0 = swapchain image
                attachments.push_back(swapChainImageViews[i]);
            }

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            if(vkCreateFramebuffer(refDevice.device, &framebufferInfo, nullptr, &swapChainFrameBuffers[i]) != VK_SUCCESS)
            {
                RBLOG_FATAL("failed to create framebuffer");
                throw::std::runtime_error("failed to create framebuffer");
            }
            RBLOG_INFO("RBSwapChain::createFrameBuffers()");
        }
    }

    void RBSwapChain::createImageView()
    {
        swapChainImageViews.resize(swapChainImages.size());
        for(size_t i = 0; i < swapChainImages.size(); i++)
        {
            swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
        }
        RBLOG_INFO("RBSwapChain::createImageView()");
    }

    void RBSwapChain::SetSwapChainExtent(RBDevice& rbDevice, RBWindows& window)
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(rbDevice.physicalDevice, rbDevice.surface);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, window.window);
        swapChainExtent = extent;
    }

    void RBSwapChain::CreateSwapChain(RBDevice& rbDevice, RBWindows& window) 
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(rbDevice.physicalDevice, rbDevice.surface);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, window.window);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = rbDevice.surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = findQueueFamilies(rbDevice.physicalDevice, &rbDevice.surface);
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};
        if(indices.graphicsFamily != indices.presentFamily){
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }
        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if(vkCreateSwapchainKHR(refDevice.device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
        {
            RBLOG_FATAL("Failed to create swap chain!");
            throw std::runtime_error("failed to create swap chain!");
            vkGetSwapchainImagesKHR(refDevice.device, swapChain, &imageCount, nullptr);
            swapChainImages.resize(imageCount);
        }
        vkGetSwapchainImagesKHR(refDevice.device, swapChain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(refDevice.device, swapChain, &imageCount, swapChainImages.data());

        swapChainImageFormat = surfaceFormat.format;

        swapChainExtent = extent;
        RBLOG_INFO("RBSwapChain::CreateSwapChain()");
    }

    void RBSwapChain::createSyncObjects()
    {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(refDevice.device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS
                || vkCreateSemaphore(refDevice.device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS
                || vkCreateFence(refDevice.device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
                RBLOG_FATAL("failed to create synchronization object for a frame!");
                throw ::std::runtime_error("failed to create synchronization object for a frame!");
            }
        }
        RBLOG_INFO("RBSwapChain::createSyncObjects()");
    }

    void RBSwapChain::InitializeSwapChain()
    {
        CreateSwapChain(refDevice, refWindow);
        createImageView();
        createColorResources();
        createDepthResources();
        createRenderPass();
        createFrameBuffers();
        createSyncObjects();
    }

    void RBSwapChain::recreateSwapChain(VkImageView* depthView)
    {
        int width = 0, height = 0;

        SDL_GetWindowSizeInPixels(refWindow.window, &width, &height);
        while (width == 0 || height == 0) {
            SDL_Event event;
            SDL_WaitEvent(&event);  // Wait for an event to ensure the window size is updated

            if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
                SDL_GetWindowSizeInPixels(refWindow.window, &width, &height);
            }
        }

        vkDeviceWaitIdle(refDevice.device);
        cleanupSwapChain();
        //depthImageView = depthView;
        CreateSwapChain(refDevice, refWindow);
        createImageView();
        createColorResources();
        createDepthResources();
        createRenderPass();
        //createGraphicsPipeline();
        createFrameBuffers();
    }

    RBSwapChain::RBSwapChain(RBDevice& device, RBWindows& window, RBCommandBuffer& commandBuffer) : refDevice(device), refWindow(window), refCommandBuffer(commandBuffer)
    {

    }

    void RBSwapChain::cleanupSwapChain()
    {
    if (colorImageView != VK_NULL_HANDLE) {
        vkDestroyImageView(refDevice.device, colorImageView, nullptr);
        colorImageView = VK_NULL_HANDLE;
    }
    if (colorImage != VK_NULL_HANDLE) {
        vkDestroyImage(refDevice.device, colorImage, nullptr);
        colorImage = VK_NULL_HANDLE;
    }
    if (colorImageMemory != VK_NULL_HANDLE) {
        vkFreeMemory(refDevice.device, colorImageMemory, nullptr);
        colorImageMemory = VK_NULL_HANDLE;
    }
    
    if (depthImageView != VK_NULL_HANDLE) {
        vkDestroyImageView(refDevice.device, depthImageView, nullptr);
        depthImageView = VK_NULL_HANDLE;
    }
    if (depthImage != VK_NULL_HANDLE) {
        vkDestroyImage(refDevice.device, depthImage, nullptr);
        depthImage = VK_NULL_HANDLE;
    }
    if (depthImageMemory != VK_NULL_HANDLE) {
        vkFreeMemory(refDevice.device, depthImageMemory, nullptr);
        depthImageMemory = VK_NULL_HANDLE;
    }
    for(auto frameBuffer : swapChainFrameBuffers)
    {
        vkDestroyFramebuffer(refDevice.device, frameBuffer, nullptr);
    }
    swapChainFrameBuffers.clear();

    if (renderPassSky != VK_NULL_HANDLE) {
        vkDestroyRenderPass(refDevice.device, renderPassSky, nullptr);
        renderPassSky = VK_NULL_HANDLE;
    }
    if (renderPass != VK_NULL_HANDLE) {
        vkDestroyRenderPass(refDevice.device, renderPass, nullptr);
        renderPass = VK_NULL_HANDLE;
    }

    for(auto imageView : swapChainImageViews)
    {
        vkDestroyImageView(refDevice.device, imageView, nullptr);
    }
    swapChainImageViews.clear();
    
    if (swapChain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(refDevice.device, swapChain, nullptr);
        swapChain = VK_NULL_HANDLE;
    }
    }

    RBSwapChain::~RBSwapChain() 
    {
        cleanupSwapChain();

        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            vkDestroySemaphore(refDevice.device, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(refDevice.device, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(refDevice.device, inFlightFences[i], nullptr);
        }
    }
}