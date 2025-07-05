#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct RBCamera 
{
    glm::vec3 position = glm::vec3(0.0f, 5.0f, 20.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

    float yaw = -90.0f;
    float pitch = 0.0f;

    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    glm::mat4 GetViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 GetProjectionMatrix(float aspect) const {
        glm::mat4 proj = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
        proj[1][1] *= -1; // Vulkan clip space correction
        return proj;
    }

    void UpdateDirectionFromYawPitch() {
        glm::vec3 dir;
        dir.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        dir.y = sin(glm::radians(pitch));
        dir.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        front = glm::normalize(dir);

        glm::vec3 worldUp = glm::vec3(0, 1, 0);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};