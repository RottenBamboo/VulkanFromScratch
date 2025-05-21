#version 450
layout(binding = 1) uniform sampler2D albedoMap;
layout(binding = 2) uniform sampler2D normalMap;
layout(binding = 3) uniform sampler2D AOMap;
layout(binding = 4) uniform sampler2D emissionMap;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 positionWS;

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gAlbedo;
layout(location = 3) out vec4 gMaterial;

void main()
{
    float AOIntensity = texture(AOMap, fragTexCoord).r;
    gPosition = vec4(positionWS, AOIntensity);//texture(albedoMap, fragTexCoord);
    gNormal= texture(normalMap, fragTexCoord);
    gAlbedo = texture(albedoMap, fragTexCoord);
    gMaterial = texture(emissionMap, fragTexCoord);
    //vec4 object = texture(texSampler, fragTexCoord);
    //float isColored = dot(object, vec4(1.0, 1.0, 1.0, 1.0));
    //outColor = isColored > 0 ? vec4(1.0, 1.0, 1.0, 0.15f) : vec4(0.0, 0.0, 0.0, 0.15f);
}