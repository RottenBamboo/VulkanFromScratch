#pragma once
#include <SDL.h>
#include "RBCamera.h"

class RBCameraManager 
{
public:
    virtual void Update(RBCamera& camera, float deltaTime) = 0;
    virtual void OnEvent(const SDL_Event& e) = 0;
    virtual ~RBCameraManager() = default;
};