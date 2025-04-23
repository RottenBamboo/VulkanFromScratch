#version 450
layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;
void main()
{
    vec4 object = texture(texSampler, fragTexCoord);
    float isColored = dot(object, vec4(1.0, 1.0, 1.0, 1.0));
    outColor = isColored > 0 ? vec4(1.0, 1.0, 1.0, 0.15f) : vec4(0.0, 0.0, 0.0, 0.15f);
}