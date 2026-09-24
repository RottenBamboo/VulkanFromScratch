#include "RBSimplePrimitive.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace RottenBamboo
{
    namespace
    {
        static void AddQuad(
            std::vector<Vertex>& vertices,
            std::vector<uint32_t>& indices,
            const glm::vec3& a,
            const glm::vec3& b,
            const glm::vec3& c,
            const glm::vec3& d,
            const glm::vec3& color)
        {
            const uint32_t baseIndex = static_cast<uint32_t>(vertices.size());

            Vertex v0{};
            v0.pos = a;
            v0.color = color;
            v0.texCoord = {0.0f, 0.0f};
            v0.normal = glm::normalize(glm::cross(b - a, c - a));
            v0.tangent = {1.0f, 0.0f, 0.0f};
            vertices.push_back(v0);

            Vertex v1{};
            v1.pos = b;
            v1.color = color;
            v1.texCoord = {1.0f, 0.0f};
            v1.normal = glm::normalize(glm::cross(b - a, c - a));
            v1.tangent = {1.0f, 0.0f, 0.0f};
            vertices.push_back(v1);

            Vertex v2{};
            v2.pos = c;
            v2.color = color;
            v2.texCoord = {1.0f, 1.0f};
            v2.normal = glm::normalize(glm::cross(b - a, c - a));
            v2.tangent = {1.0f, 0.0f, 0.0f};
            vertices.push_back(v2);

            Vertex v3{};
            v3.pos = d;
            v3.color = color;
            v3.texCoord = {0.0f, 1.0f};
            v3.normal = glm::normalize(glm::cross(b - a, c - a));
            v3.tangent = {1.0f, 0.0f, 0.0f};
            vertices.push_back(v3);

            indices.push_back(baseIndex + 0);
            indices.push_back(baseIndex + 1);
            indices.push_back(baseIndex + 2);
            indices.push_back(baseIndex + 0);
            indices.push_back(baseIndex + 2);
            indices.push_back(baseIndex + 3);
        }
    }

    RBSimplePrimitive::RBSimplePrimitive(RBDevice& device, RBCommandBuffer& commandBuffer)
        : m_Device(device), m_CommandBuffer(commandBuffer)
    {
    }

    std::unique_ptr<RBMesh> RBSimplePrimitive::CreateCube(float size)
    {
        const float half = size * 0.5f;
        const glm::vec3 color(0.8f, 0.8f, 0.8f);

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        AddQuad(vertices, indices, {-half, -half,  half}, { half, -half,  half}, { half,  half,  half}, {-half,  half,  half}, color);
        AddQuad(vertices, indices, { half, -half, -half}, {-half, -half, -half}, {-half,  half, -half}, { half,  half, -half}, color);
        AddQuad(vertices, indices, {-half, -half, -half}, {-half, -half,  half}, {-half,  half,  half}, {-half,  half, -half}, color);
        AddQuad(vertices, indices, { half, -half,  half}, { half, -half, -half}, { half,  half, -half}, { half,  half,  half}, color);
        AddQuad(vertices, indices, {-half,  half,  half}, { half,  half,  half}, { half,  half, -half}, {-half,  half, -half}, color);
        AddQuad(vertices, indices, {-half, -half, -half}, { half, -half, -half}, { half, -half,  half}, {-half, -half,  half}, color);

        auto mesh = std::make_unique<RBMesh>("primitive_cube", m_Device, m_CommandBuffer);
        mesh->AddMeshData(vertices, indices);
        mesh->InitializeMesh();

        m_mesh = std::move(mesh);
        return std::move(m_mesh);
    }

    std::unique_ptr<RBMesh> RBSimplePrimitive::CreateSphere(float radius, uint32_t segments, uint32_t rings)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        const glm::vec3 color(0.7f, 0.7f, 1.0f);

        for (uint32_t ring = 0; ring <= rings; ++ring)
        {
            const float v = static_cast<float>(ring) / static_cast<float>(rings);
            const float theta = v * glm::pi<float>();
            const float sinTheta = std::sin(theta);
            const float cosTheta = std::cos(theta);

            for (uint32_t seg = 0; seg <= segments; ++seg)
            {
                const float u = static_cast<float>(seg) / static_cast<float>(segments);
                const float phi = u * glm::two_pi<float>();
                const float sinPhi = std::sin(phi);
                const float cosPhi = std::cos(phi);

                Vertex vertex{};
                vertex.pos = {
                    radius * cosPhi * sinTheta,
                    radius * cosTheta,
                    radius * sinPhi * sinTheta
                };
                vertex.color = color;
                vertex.texCoord = {u, v};
                vertex.normal = glm::normalize(vertex.pos);
                vertex.tangent = {-sinPhi, 0.0f, cosPhi};
                vertices.push_back(vertex);
            }
        }

        for (uint32_t ring = 0; ring < rings; ++ring)
        {
            for (uint32_t seg = 0; seg < segments; ++seg)
            {
                const uint32_t first = ring * (segments + 1) + seg;
                const uint32_t second = first + segments + 1;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }

        auto mesh = std::make_unique<RBMesh>("primitive_sphere", m_Device, m_CommandBuffer);
        mesh->AddMeshData(vertices, indices);
        mesh->InitializeMesh();

        m_mesh = std::move(mesh);
        return std::move(m_mesh);
    }

    std::unique_ptr<RBMesh> RBSimplePrimitive::CreateCylinder(float radius, float height, uint32_t segments)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        const glm::vec3 color(0.8f, 0.5f, 0.3f);
        const float halfHeight = height * 0.5f;

        for (uint32_t i = 0; i <= segments; ++i)
        {
            const float angle = (static_cast<float>(i) / static_cast<float>(segments)) * glm::two_pi<float>();
            const float x = std::cos(angle) * radius;
            const float z = std::sin(angle) * radius;

            Vertex top{};
            top.pos = {x, halfHeight, z};
            top.color = color;
            top.texCoord = {static_cast<float>(i) / static_cast<float>(segments), 1.0f};
            top.normal = {0.0f, 1.0f, 0.0f};
            top.tangent = {1.0f, 0.0f, 0.0f};
            vertices.push_back(top);

            Vertex bottom{};
            bottom.pos = {x, -halfHeight, z};
            bottom.color = color;
            bottom.texCoord = {static_cast<float>(i) / static_cast<float>(segments), 0.0f};
            bottom.normal = {0.0f, -1.0f, 0.0f};
            bottom.tangent = {1.0f, 0.0f, 0.0f};
            vertices.push_back(bottom);
        }

        for (uint32_t i = 0; i < segments; ++i)
        {
            const uint32_t i0 = i * 2;
            const uint32_t i1 = i0 + 1;
            const uint32_t i2 = (i + 1) * 2;
            const uint32_t i3 = i2 + 1;

            indices.push_back(i0);
            indices.push_back(i2);
            indices.push_back(i1);
            indices.push_back(i1);
            indices.push_back(i2);
            indices.push_back(i3);
        }

        const uint32_t centerTop = static_cast<uint32_t>(vertices.size());
        Vertex topCenter{};
        topCenter.pos = {0.0f, halfHeight, 0.0f};
        topCenter.color = color;
        topCenter.texCoord = {0.5f, 1.0f};
        topCenter.normal = {0.0f, 1.0f, 0.0f};
        topCenter.tangent = {1.0f, 0.0f, 0.0f};
        vertices.push_back(topCenter);

        const uint32_t centerBottom = static_cast<uint32_t>(vertices.size());
        Vertex bottomCenter{};
        bottomCenter.pos = {0.0f, -halfHeight, 0.0f};
        bottomCenter.color = color;
        bottomCenter.texCoord = {0.5f, 0.0f};
        bottomCenter.normal = {0.0f, -1.0f, 0.0f};
        bottomCenter.tangent = {1.0f, 0.0f, 0.0f};
        vertices.push_back(bottomCenter);

        for (uint32_t i = 0; i < segments; ++i)
        {
            const uint32_t a = i * 2;
            const uint32_t b = ((i + 1) % segments) * 2;

            indices.push_back(centerTop);
            indices.push_back(b);
            indices.push_back(a);

            indices.push_back(centerBottom);
            indices.push_back(a + 1);
            indices.push_back(b + 1);
        }

        auto mesh = std::make_unique<RBMesh>("primitive_cylinder", m_Device, m_CommandBuffer);
        mesh->AddMeshData(vertices, indices);
        mesh->InitializeMesh();

        m_mesh = std::move(mesh);
        return std::move(m_mesh);
    }

    std::unique_ptr<RBMesh> RBSimplePrimitive::CreateCapsule(float radius, float height, uint32_t segments, uint32_t rings)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        const glm::vec3 color(0.9f, 0.5f, 0.8f);

        for (uint32_t ring = 0; ring <= rings; ++ring)
        {
            const float v = static_cast<float>(ring) / static_cast<float>(rings);
            const float theta = (v - 0.5f) * glm::pi<float>();
            const float sinTheta = std::sin(theta);
            const float cosTheta = std::cos(theta);

            for (uint32_t seg = 0; seg <= segments; ++seg)
            {
                const float u = static_cast<float>(seg) / static_cast<float>(segments);
                const float phi = u * glm::two_pi<float>();
                const float sinPhi = std::sin(phi);
                const float cosPhi = std::cos(phi);

                const float xzRadius = radius * std::cos(theta);
                const float y = height * 0.5f * 0.5f + radius * std::sin(theta);

                Vertex vertex{};
                vertex.pos = {
                    xzRadius * cosPhi,
                    y,
                    xzRadius * sinPhi
                };
                vertex.color = color;
                vertex.texCoord = {u, v};
                vertex.normal = glm::normalize(vertex.pos);
                vertex.tangent = {-sinPhi, 0.0f, cosPhi};
                vertices.push_back(vertex);
            }
        }

        for (uint32_t ring = 0; ring < rings; ++ring)
        {
            for (uint32_t seg = 0; seg < segments; ++seg)
            {
                const uint32_t a = ring * (segments + 1) + seg;
                const uint32_t b = a + segments + 1;
                const uint32_t c = a + 1;
                const uint32_t d = b + 1;

                indices.push_back(a);
                indices.push_back(b);
                indices.push_back(c);

                indices.push_back(b);
                indices.push_back(d);
                indices.push_back(c);
            }
        }

        auto mesh = std::make_unique<RBMesh>("primitive_capsule", m_Device, m_CommandBuffer);
        mesh->AddMeshData(vertices, indices);
        mesh->InitializeMesh();

        m_mesh = std::move(mesh);
        return std::move(m_mesh);
    }

    std::unique_ptr<RBMesh> RBSimplePrimitive::CreatePlane(float width, float height)
    {
        const float halfW = width * 0.5f;
        const float halfH = height * 0.5f;

        std::vector<Vertex> vertices = {
            {{-halfW, 0.0f, -halfH}, {0.8f, 0.8f, 0.8f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{ halfW, 0.0f, -halfH}, {0.8f, 0.8f, 0.8f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{ halfW, 0.0f,  halfH}, {0.8f, 0.8f, 0.8f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{-halfW, 0.0f,  halfH}, {0.8f, 0.8f, 0.8f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        };

        std::vector<uint32_t> indices = {0, 1, 2, 0, 2, 3};

        auto mesh = std::make_unique<RBMesh>("primitive_plane", m_Device, m_CommandBuffer);
        mesh->AddMeshData(vertices, indices);
        mesh->InitializeMesh();

        m_mesh = std::move(mesh);
        return std::move(m_mesh);
    }
}