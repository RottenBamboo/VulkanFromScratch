#include "RBTransform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace RottenBamboo
{
    void Transform::UpdateModelMatrix()
    {
        transformData.model = glm::translate(matrix4x4(1.0f), transformData.offset)
            * glm::mat4_cast(transformData.myQuat)
            * glm::scale(matrix4x4(1.0f), transformData.scale);
    }

    void Transform::DecomposeModelMatrix()
    {
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::quat rot;
        glm::vec3 scale;
        glm::vec3 pos;

        glm::decompose(transformData.model, scale, rot, pos, skew, perspective);

        transformData.offset = pos;
        transformData.scale = scale;
        transformData.myQuat = glm::normalize(rot);
        transformData.euler = glm::eulerAngles(transformData.myQuat);
    }

    vector3 Transform::GetScale() const
    {
        return transformData.scale;
    }

    void Transform::SetScale(const vector3& scale)
    {
        transformData.scale = scale;
        UpdateModelMatrix();
    }

    vector3 Transform::GetPosition() const
    {
        return transformData.offset;
    }

    void Transform::SetPosition(const vector3& pos)
    {
        transformData.offset = pos;
        UpdateModelMatrix();
    }

    quat Transform::GetRotation() const
    {
        return transformData.myQuat;
    }

    void Transform::SetRotation(const quat& rot)
    {
        transformData.myQuat = glm::normalize(rot);
        transformData.euler = glm::eulerAngles(transformData.myQuat);
        UpdateModelMatrix();
    }

    vector3 Transform::GetEuler() const
    {
        return transformData.euler;
    }

    void Transform::SetEuler(const vector3& euler)
    {
        transformData.euler = euler;
        transformData.myQuat = quat(euler);
        UpdateModelMatrix();
    }

    matrix4x4 Transform::GetModelMatrix() const
    {
        return transformData.model;
    }

    void Transform::SetModelMatrix(const matrix4x4* modelMatrix)
    {
        transformData.model = *modelMatrix;
        DecomposeModelMatrix();
    }

    Transform::Transform(const TransformData& data) : transformData(data)
    {
        const float quatLenSq = transformData.myQuat.x * transformData.myQuat.x +
            transformData.myQuat.y * transformData.myQuat.y +
            transformData.myQuat.z * transformData.myQuat.z +
            transformData.myQuat.w * transformData.myQuat.w;

        if (quatLenSq <= 0.0f)
        {
            transformData.myQuat = quat(1.0f, 0.0f, 0.0f, 0.0f);
        }
        transformData.myQuat = glm::normalize(transformData.myQuat);
        transformData.euler = glm::eulerAngles(transformData.myQuat);
        UpdateModelMatrix();
    }

    Transform::Transform(const vector3& pos, const vector3& scale, const quat& rot)
    {
        transformData.offset = pos;
        transformData.scale = scale;
        transformData.myQuat = glm::normalize(rot);
        transformData.euler = glm::eulerAngles(transformData.myQuat);
        UpdateModelMatrix();
    }
}
