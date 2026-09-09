#include "RBTransform.h"
namespace RottenBamboo
{
    vector3 Transform::GetScale() const
    {
        return transformData.scale;
    }

    void Transform::SetScale(const vector3& scale)
    {
        transformData.scale = scale;
    }

    vector3 Transform::GetPosition() const
    {
        return transformData.offset;
    }

    void Transform::SetPosition(const vector3& pos)
    {
        transformData.offset = pos;
    }

    quat Transform::GetRotation() const
    {
        return transformData.myQuat;
    }

    void Transform::SetRotation(const quat& rot)
    {
        transformData.myQuat = rot;
    }

    vector3 Transform::GetEuler() const
    {
        return transformData.euler;
    }

    void Transform::SetEuler(const vector3& euler)
    {
        transformData.euler = euler;
    }

    matrix4x4 Transform::GetModelMatrix() const
    {
        return transformData.model;
    }

    void Transform::SetModelMatrix(const matrix4x4* modelMatrix)
    {
        transformData.model = *modelMatrix;
    }

    Transform::Transform(const TransformData& data) : transformData(data) 
    {

    }
    Transform::Transform(const vector3& pos, const vector3& scale, const quat& rot)
    {
        transformData.offset = pos;
        transformData.scale = scale;
        transformData.myQuat = rot;
        transformData.euler = glm::eulerAngles(rot);
        transformData.model = glm::translate(matrix4x4(1.0f), pos) * glm::mat4(rot) * glm::scale(matrix4x4(1.0f), scale);
    }
}
        