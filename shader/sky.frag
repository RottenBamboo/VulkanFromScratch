#version 450
layout(binding = 1) uniform sampler2D albedoMap;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main()
{
    vec4 baseColor = texture(albedoMap, fragTexCoord);
    outColor = vec4(baseColor.rgb, 1.0);
}