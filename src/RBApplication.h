//
// Created by rottenbamboo on 2023/5/16.
//

#pragma once

#include "RBWindows.h"
#include "RBDevice.h"
#include "RBCommandBuffer.h"
#include "RBSwapChain.h"
#include "RBGBufferPass.h"
#include "RBSkyPass.h"
#include "RBLightingPass.h"
#include "RBBuffer.h"
#include "RBResourceManager.h"
#include "RBResourceShader.h"
#include "RBRuntimeCameraManager.h"
#include "RBEditorCameraManager.h"
#include "RBShaderDefinition.h"
#include <stdexcept>
#include <iostream>
#include <string>
#include <queue>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <chrono>
#include "uuid.h"
#include <efsw/efsw.hpp>
#include "RBAssetsRegistry.h"
#include "RBAssetsRegistryListener.h"
#include "RBScene.h"

namespace RottenBamboo 
{
    
    class RBApplication 
    {

    public:
        static RBApplication* instance;
        static const uint32_t WIDTH = 1920;
        static const uint32_t HEIGHT = 1080;

        void run();

        void drawFrame();

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        RBApplication();

        ~RBApplication();

        void InitializeWindow();

        void InitializeCamera();

        void InitializeDevice();

        void InitializeCommandBuffer();

        void InitializeSwapChain();

        void InitializeFileListener();

        void transformModelVertex(aiMesh* meshPtr, 
                                          std::vector<Vertex>& vertexBuffer, 
                                          int& vertexWriteIndex, 
                                          const aiMatrix4x4& transform);

        void processModelNode(const aiNode* node,
                         const aiScene* scene,
                         std::vector<Vertex>& vertexBuffer,
                         std::vector<uint32_t>& indexBuffer,
                         int& vertexWriteIndex,
                         int& vertexStartOffset,
                         const aiMatrix4x4& parentTransform);

        void InitializeBuffers();

        void InitializeDescriptors();

        void CreateNewDescriptors(std::vector<RBMaterial>& materials, uuids::uuid& guidVertex, uuids::uuid& guidFragment);

        void InitializeGraphicPipeline();

        void InitializeGUI();

        void InitializeScene();

        void InitializeMatrix();

        void UpdateSceneTransform(RBSceneNode* root, vector3 pos, vector3 scale, quat);

        void CopySceneTransform(RBSceneNode* root, int frameIndex, UniformBufferShaderVariables* uniformShaderVariables);
        
        void InitializeMaterial();
        
        void InitializeEditorMaterial();
        
        void InitializeShaderDefinition();

        void updateDirtyDescriptorSets();
        
        void processFileEvents();

        std::shared_ptr<RBMaterial> AddMaterial(const std::string& materialPath = "");

    private:
        //file event
        std::unique_ptr<efsw::FileWatcher> m_FileWatcher;
        std::unique_ptr<RBAssetsRegistryListener> m_FileListener;
        std::queue<std::string> m_FileEventQueue;
        std::mutex m_EventMutex;
    public:

        RBAssetsRegistry assetsRegistry;
        
        std::chrono::high_resolution_clock::time_point lastFrameTime;

        RBWindows windows{WIDTH, HEIGHT, "Vulkan"};

        RBCamera mainCamera;

        std::unique_ptr<RBCameraManager> cameraManager;

        RBDevice device{windows};

        RBCommandBuffer commandBuffer{device};

        RBGUI gui{device, windows, commandBuffer};

        std::unordered_map<uuids::uuid, RBShaderDefinition> shaderDefinitions;

        RBScene scene{defaultSceneName};

        RBMaterial editorMaterial{"", device, commandBuffer};

        ResourceManager resourceManager{device, commandBuffer};

        RBResourceShader resourceShader{DEFAULT_SHADER_REFLECTION};

        RBPipelineConfig pipelineConfig{};

        RBSimplePrimitive m_simplePrimitive{device, commandBuffer};

        std::vector<RBDescriptors*> m_pDescriptorsGBuffersVec;
        std::vector<RBMaterial> appendMaterialsVec;

        static std::vector<ImageResourcePtr> oldImageResourceVec;
        
        std::vector<std::unique_ptr<RBDescriptors>> m_descriptorsGBuffersVec;

        RBDescriptors descriptorsGBuffer{device, commandBuffer, inputImageInfoMech, false, uniformBuffers1};

        RBDescriptors descriptorsSkyBox{device, commandBuffer, inputImageInfoSkyBox, false, uniformBuffers1};

        RBDescriptors descriptorsAttachment{device, commandBuffer, true, uniformBuffers1};
        RBDescriptors descriptorsLighting{device, commandBuffer, inputImageInfoLighting, true, uniformBuffers1};

        RBSwapChain swapChain{device, windows, commandBuffer};
        
        RBGBufferPass gBufferPass{gBufferPassAttachmentCount, false, true, device, descriptorsGBuffer, descriptorsAttachment, pipelineConfig, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

        RBSkyPass skyPassManager{skyBoxPassColorAttachmentCount, false, false, device, descriptorsSkyBox, pipelineConfig, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

        RBLightingPass lightPassManager{lightPassColorAttachmentCount, true, false, device, descriptorsLighting, pipelineConfig, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

        //RBBuffer<UniformBufferShaderVariables> uniformBuffers[MAX_FRAMES_IN_FLIGHT]{{device, commandBuffer, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT}, {device, commandBuffer, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT}};

        RBBuffer<SceneUBO> uniformBuffers1[MAX_FRAMES_IN_FLIGHT]{{device, commandBuffer, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT}, {device, commandBuffer, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT}};

        UniformBufferShaderVariables uniformShaderVariables{};

        void updateUniformBuffer(uint32_t currentImage);
        void InitializeStaticPtr();

    public:
    
        static RBScene* ptr_scene;

        static RBScene* GetScene();
    
        static RBSimplePrimitive* ptr_simplePrimitive;

        static RBSimplePrimitive* GetSimplePrimitive();
    
        static ResourceManager* ptr_resourceManager;

        static ResourceManager* GetResourceManager();
    
        static RBAssetsRegistry* ptr_assetsRegistry;

        static RBAssetsRegistry* GetAssetRegistry();

        static RBGUI* ptr_gui;

        static RBGUI* GetGUI();

        static RBResourceShader* ptr_resourceShader;

        static RBResourceShader* GetResourceShader();

        static std::unordered_map<uuids::uuid, RBShaderDefinition>* ptr_shaderDefinition;

        static std::unordered_map<uuids::uuid, RBShaderDefinition>* GetShaderDefinition();

        static RBShaderDefinition* GetShaderDefinition(const uuids::uuid& guid);

        static std::vector<RBDescriptors*>* ptr_Descriptors;

        static std::vector<RBDescriptors*>* GetDescriptors();

        static std::vector<RBDescriptors*> updatedDescriptors;

        static std::vector<RBDescriptors*>* GetUpdatedDescriptors();

        static std::vector<ImageResourcePtr>* GetOldImageResource();

        static bool descriptorSetsUpdate;
        
        int deferredFrameCount = 0;
    };
}
