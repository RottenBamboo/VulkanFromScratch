#pragma once

#include "RBMesh.h"
#include <memory>

namespace RottenBamboo
{

    enum SimplePrimitiveType
    {
        TYPE_CUBE,
        TYPE_SPHERE,
        TYPE_CYLINDER,
        TYPE_CAPSULE,
        TYPE_PLANE,
        TYPE_MAX
    };
    struct SimplePrimitiveData
    {
        SimplePrimitiveType type;
        float width;
        float height;
        float radius;
        float segments;
        float rings;
    };

    class RBSimplePrimitive
    {
    public:
        RBSimplePrimitive(RBDevice& device, RBCommandBuffer& commandBuffer);

        std::unique_ptr<RBMesh> CreateCube(float size = 1.0f);
        std::unique_ptr<RBMesh> CreateSphere(float radius = 0.5f, uint32_t segments = 32, uint32_t rings = 16);
        std::unique_ptr<RBMesh> CreateCylinder(float radius = 0.5f, float height = 1.0f, uint32_t segments = 32);
        std::unique_ptr<RBMesh> CreateCapsule(float radius = 0.5f, float height = 1.0f, uint32_t segments = 32, uint32_t rings = 16);
        std::unique_ptr<RBMesh> CreatePlane(float width = 1.0f, float height = 1.0f);
        std::unique_ptr<RBMesh> CreateSimplePrimitive(SimplePrimitiveData* data);
        const std::unique_ptr<RBMesh>& GetMesh() const { return m_mesh; }

    private:
        RBDevice& m_Device;
        RBCommandBuffer& m_CommandBuffer;
        std::unique_ptr<RBMesh> m_mesh;
    };
}