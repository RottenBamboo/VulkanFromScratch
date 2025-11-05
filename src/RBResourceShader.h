//
// Create by rottenbamboo on 2025/11/5.
//
#pragma once

#include "RBResource.h"
namespace RottenBamboo {

    class RBResourceShader : public RBResource {
    public:
        RBResourceShader(const std::string &path) : RBResource(path) {}
        ~RBResourceShader() = default;

        void Load(const std::string& path) override 
        {
            
        }
    };
}