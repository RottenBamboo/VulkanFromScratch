#include "RBTransform.h"

vector3 RottenBamboo::Transform::GetScale() const
{
    return data.scale;
}

void RottenBamboo::Transform::SetScale(const vector3& scale)
{
    data.scale = scale;
}

vector3 RottenBamboo::Transform::GetPosition() const
{
    return data.offset;
}

void RottenBamboo::Transform::SetPosition(const vector3& pos)
{
    data.offset = pos;
}

quat RottenBamboo::Transform::GetRotation() const
{
    return data.myQuat;
}

void RottenBamboo::Transform::SetRotation(const quat& rot)
{
    data.myQuat = rot;
}

vector3 RottenBamboo::Transform::GetEuler() const
{
    return data.euler;
}

void RottenBamboo::Transform::SetEuler(const vector3& euler)
{
    data.euler = euler;
}

matrix4x4 RottenBamboo::Transform::GetModelMatrix() const
{
    return data.model;
}

void RottenBamboo::Transform::SetModelMatrix(const matrix4x4* modelMatrix)
{
    data.model = *modelMatrix;
}