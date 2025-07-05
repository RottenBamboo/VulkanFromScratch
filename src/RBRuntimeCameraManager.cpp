#include "RBRuntimeCameraManager.h"
#include <glm/gtx/hash.hpp>
RBRuntimeCameraManager::RBRuntimeCameraManager() {}

void RBRuntimeCameraManager::OnEvent(const SDL_Event& e) 
{
    if (e.type == SDL_EVENT_KEY_DOWN)
        keys[e.key.scancode] = true;
    else if (e.type == SDL_EVENT_KEY_UP)
        keys[e.key.scancode] = false;
    else if (e.type == SDL_EVENT_MOUSE_MOTION && (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_RMASK)) {
        yawOffset   += e.motion.xrel * sensitivity;
        pitchOffset -= e.motion.yrel * sensitivity;
    }
}

void RBRuntimeCameraManager::Update(RBCamera& camera, float deltaTime) 
{
    camera.yaw += yawOffset;
    camera.pitch += pitchOffset;
    camera.pitch = glm::clamp(camera.pitch, -89.0f, 89.0f);
    yawOffset = pitchOffset = 0.0f;

    camera.UpdateDirectionFromYawPitch();

    glm::vec3 move = glm::vec3(0.0f);
    if (keys[SDL_SCANCODE_W]) move += camera.front;
    if (keys[SDL_SCANCODE_S]) move -= camera.front;
    if (keys[SDL_SCANCODE_A]) move -= camera.right;
    if (keys[SDL_SCANCODE_D]) move += camera.right;
    if (keys[SDL_SCANCODE_Q]) move -= camera.up;
    if (keys[SDL_SCANCODE_E]) move += camera.up;

    camera.position += move * moveSpeed * deltaTime;
}
