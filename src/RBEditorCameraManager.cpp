#include "RBEditorCameraManager.h"

RBEditorCameraManager::RBEditorCameraManager() {}

void RBEditorCameraManager::OnEvent(const SDL_Event& e) 
{
    if (e.type == SDL_EVENT_KEY_DOWN)
        keys[e.key.scancode] = true;
    else if (e.type == SDL_EVENT_KEY_UP)
        keys[e.key.scancode] = false;
    else if (e.type == SDL_EVENT_MOUSE_MOTION && (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_RMASK)) {
        yawOffset   += e.motion.xrel * sensitivity;
        pitchOffset -= e.motion.yrel * sensitivity;
    }
    else if (e.type == SDL_EVENT_MOUSE_WHEEL) {
        distance *= 1.0f - e.wheel.y * 0.1f;
        distance = glm::clamp(distance, 1.0f, 50.0f);
    }
}

void RBEditorCameraManager::Update(RBCamera& camera, float deltaTime) 
{
    yaw += yawOffset;
    pitch += pitchOffset;
    pitch = glm::clamp(pitch, -89.0f, 89.0f);
    yawOffset = pitchOffset = 0.0f;

    float pitchRad = glm::radians(pitch);
    float yawRad = glm::radians(yaw);

    glm::vec3 offset;
    offset.x = cos(pitchRad) * cos(yawRad);
    offset.y = sin(pitchRad);
    offset.z = cos(pitchRad) * sin(yawRad);

    camera.position = focus - offset * distance;
    camera.front = glm::normalize(focus - camera.position);
    camera.right = glm::normalize(glm::cross(camera.front, glm::vec3(0, 1, 0)));
    camera.up = glm::normalize(glm::cross(camera.right, camera.front));

    glm::vec3 move(0.0f);
    if (keys[SDL_SCANCODE_W]) move += camera.front;
    if (keys[SDL_SCANCODE_S]) move -= camera.front;
    if (keys[SDL_SCANCODE_A]) move -= camera.right;
    if (keys[SDL_SCANCODE_D]) move += camera.right;
    if (keys[SDL_SCANCODE_Q]) move -= camera.up;
    if (keys[SDL_SCANCODE_E]) move += camera.up;

    if (glm::length(move) > 0.0f) {
        focus += glm::normalize(move) * moveSpeed * deltaTime;
    }
}