#version 450
layout(binding = 0) uniform sampler2D gBufferAlbedo;
layout(binding = 1) uniform sampler2D gBufferNormal;
layout(binding = 2) uniform sampler2D gBufferAO;
layout(binding = 3) uniform sampler2D gBufferEmission;




layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;
void main()
{
    outColor = texture(gBufferAlbedo, fragTexCoord);
    outColor.a = 1.0f;
}