#version 450
layout(binding = 1) uniform sampler2D gPosition;
layout(binding = 2) uniform sampler2D gNormal;
layout(binding = 3) uniform sampler2D gAlbedo;
layout(binding = 4) uniform sampler2D gMaterial;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;
void main()
{
    outColor = texture(gPosition, fragTexCoord);
    outColor.a = 1.0f;
}