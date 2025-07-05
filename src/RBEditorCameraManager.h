#pragma once

#include <SDL3/SDL_scancode.h>
#include "RBCameraManager.h"

class RBEditorCameraManager : public RBCameraManager {
public:
    RBEditorCameraManager();

    void Update(RBCamera& camera, float deltaTime) override;
    void OnEvent(const SDL_Event& e) override;

    float moveSpeed = 5.0f;
    float sensitivity = 0.1f;

private:
    bool keys[SDL_SCANCODE_COUNT] = {};
    float yaw = -90.0f;
    float pitch = 0.0f;
    float distance = 5.0f;
    glm::vec3 focus = glm::vec3(0.0f);

    float yawOffset = 0.0f;
    float pitchOffset = 0.0f;
};