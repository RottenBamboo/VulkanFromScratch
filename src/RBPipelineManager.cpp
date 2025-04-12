#include "RBPipelineManager.h"
#include <iostream>

namespace RottenBamboo {

    RBPipelineManager::RBPipelineManager() 
    {
        std::cout << "RBPipelineManager::RBPipelineManager()" << std::endl;
    }
    void RBPipelineManager::createGraphicsPipeline() 
    {
        setupShaders();
        setupPipelineStates();
        std::cout << "RBPipelineManager::createGraphicsPipeline()" << std::endl;
    }

    void RBPipelineManager::setupShaders()
    {
        // Implementation for setting up shaders
        std::cout << "RBPipelineManager::setupShaders()" << std::endl;
    }

    void RBPipelineManager::setupPipelineStates()
    {
        // Implementation for setting up pipeline states
        std::cout << "RBPipelineManager::setupPipelineStates()" << std::endl;
    }
    
}