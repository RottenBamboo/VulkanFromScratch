#pragma once
#include <SDL_scancode.h>
#include "RBCameraManager.h"

class RBRuntimeCameraManager : public RBCameraManager {
public:
    RBRuntimeCameraManager();

    void Update(RBCamera& camera, float deltaTime) override;
    void OnEvent(const SDL_Event& e) override;

    float moveSpeed = 5.0f;
    float sensitivity = 0.1f;

private:
    bool keys[SDL_SCANCODE_COUNT] = {};
    float yawOffset = 0.0f;
    float pitchOffset = 0.0f;
};
