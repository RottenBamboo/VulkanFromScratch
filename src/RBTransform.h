#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
using vector4 = glm::vec4;
using vector3 = glm::vec3;
using vector2 = glm::vec2;
using vector1 = glm::vec1;
using matrix4x4 = glm::mat4;
using matrix3x3 = glm::mat3;
using lowp_quat = glm::lowp_quat;
using mediump_quat = glm::mediump_quat;
using highp_quat = glm::highp_quat;
using quat = glm::quat;

namespace RottenBamboo
{
    struct TransformData
    {
        vector3 offset = vector3(0, 0, 0);
        vector3 scale = {1, 1, 1};
        quat myQuat = quat(offset);
        vector3 euler = {0, 0, 0};
        matrix4x4 model;
    };
    
    class Transform
    {

        TransformData data;
        void SetScale(const vector3& scale);
        vector3 GetScale() const;
        void SetPosition(const vector3& pos);
        vector3 GetPosition() const;
        void SetRotation(const quat& rot);
        quat GetRotation() const;
        void SetEuler(const vector3& euler);
        vector3 GetEuler() const;
        void SetModelMatrix(const matrix4x4* modelMatrix);
        matrix4x4 GetModelMatrix() const;
    };
}