#version 450
layout(binding = 1) uniform sampler2D albedoMap;
layout(binding = 2) uniform sampler2D normalMap;
layout(binding = 3) uniform sampler2D AOMap;
layout(binding = 4) uniform sampler2D EmissionMap;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 0) out vec4 outAlbedo;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outAO;
layout(location = 3) out vec4 outEmission;
void main()
{
    outAlbedo = vec4(1,0,0,1);//texture(albedoMap, fragTexCoord);
    outNormal = vec4(1,1,1,1);
    outAO = vec4(1,1,1,1);
    outEmission = vec4(1,1,1,1);
    //vec4 object = texture(texSampler, fragTexCoord);
    //float isColored = dot(object, vec4(1.0, 1.0, 1.0, 1.0));
    //outColor = isColored > 0 ? vec4(1.0, 1.0, 1.0, 0.15f) : vec4(0.0, 0.0, 0.0, 0.15f);
}