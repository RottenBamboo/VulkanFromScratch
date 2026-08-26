#pragma once

//
// Created by rottenbamboo on 2025/10/13.
//
#include <string>
namespace RottenBamboo {

    class RBResource {
    public:
        explicit RBResource(const std::string& path) : path(path) {}
        virtual void Load(const std::string& path) = 0;
        const std::string& GetPath() const { return path; }
        void SetPath(const std::string& newPath) { path = newPath; }
        //virtual void* Get() = 0;
        virtual ~RBResource() = default;
    protected:
        std::string path;
    };
}