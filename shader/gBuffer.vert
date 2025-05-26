#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in vec3 inTangent;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 positionWS;
layout(location = 3) out vec3 fragNormal;
layout(location = 4) out vec3 fragTangent;
layout(location = 5) out vec3 fragBitangent;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

void main()
{
    vec4 worldPos = ubo.model * vec4(inPosition, 1.0);
    positionWS = worldPos.xyz;

    gl_Position = ubo.proj * ubo.view * worldPos;

    fragColor = inColor;
    fragTexCoord = inTexCoord;

    // calculate the normal matrix
    // normalMatrix = transpose(inverse(mat3(ubo.model)));
    mat3 normalMatrix = transpose(inverse(mat3(ubo.model)));

    // transform normals and tangents to world space
    fragNormal = normalize(normalMatrix * inNormal);
    fragTangent = normalize(normalMatrix * inTangent);

    // calculate the bitangent
    fragBitangent = normalize(cross(fragNormal, fragTangent));

}