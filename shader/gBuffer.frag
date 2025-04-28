#version 450
layout(binding = 1) uniform sampler2D texSampler0;
layout(binding = 2) uniform sampler2D texSampler1;
layout(binding = 3) uniform sampler2D texSampler2;
layout(binding = 4) uniform sampler2D texSampler3;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;
void main()
{
    vec3 color = texture(texSampler0, fragTexCoord).xyz;
    color += texture(texSampler1, fragTexCoord).xyz;
    color += texture(texSampler2, fragTexCoord).xyz;
    color += texture(texSampler3, fragTexCoord).xyz;
    color *= 0.25f;
    outColor.xyz = color;
    outColor.w = 1;
    //vec4 object = texture(texSampler, fragTexCoord);
    //float isColored = dot(object, vec4(1.0, 1.0, 1.0, 1.0));
    //outColor = isColored > 0 ? vec4(1.0, 1.0, 1.0, 0.15f) : vec4(0.0, 0.0, 0.0, 0.15f);
}