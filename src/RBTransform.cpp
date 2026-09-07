#include "RBTransform.h"

vector3 RottenBamboo::Transform::GetScale() const
{
    return transformData.scale;
}

void RottenBamboo::Transform::SetScale(const vector3& scale)
{
    transformData.scale = scale;
}

vector3 RottenBamboo::Transform::GetPosition() const
{
    return transformData.offset;
}

void RottenBamboo::Transform::SetPosition(const vector3& pos)
{
    transformData.offset = pos;
}

quat RottenBamboo::Transform::GetRotation() const
{
    return transformData.myQuat;
}

void RottenBamboo::Transform::SetRotation(const quat& rot)
{
    transformData.myQuat = rot;
}

vector3 RottenBamboo::Transform::GetEuler() const
{
    return transformData.euler;
}

void RottenBamboo::Transform::SetEuler(const vector3& euler)
{
    transformData.euler = euler;
}

matrix4x4 RottenBamboo::Transform::GetModelMatrix() const
{
    return transformData.model;
}

void RottenBamboo::Transform::SetModelMatrix(const matrix4x4* modelMatrix)
{
    transformData.model = *modelMatrix;
}