#pragma once
namespace RottenBamboo {
class RBPipelineManager {
    protected:
        virtual void setupShaders() = 0;
        virtual void setupPipelineStates();
    
    public:

        RBPipelineManager();
    
        virtual ~RBPipelineManager() = default;

        void createGraphicsPipeline();
    };
}