#version 450
layout(binding = 1) uniform sampler2D texSampler0;
layout(binding = 2) uniform sampler2D texSampler1;
layout(binding = 3) uniform sampler2D texSampler2;
layout(binding = 4) uniform sampler2D texSampler3;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gAlbedo;
layout(location = 3) out vec4 gMaterial;

void main()
{
    gPosition = texture(texSampler0, fragTexCoord);
    gNormal= texture(texSampler1, fragTexCoord);
    gAlbedo = texture(texSampler2, fragTexCoord);
    gMaterial = texture(texSampler3, fragTexCoord);
    //vec4 object = texture(texSampler, fragTexCoord);
    //float isColored = dot(object, vec4(1.0, 1.0, 1.0, 1.0));
    //outColor = isColored > 0 ? vec4(1.0, 1.0, 1.0, 0.15f) : vec4(0.0, 0.0, 0.0, 0.15f);
}