#version 450
#extension GL_GOOGLE_include_directive : require
#include "common/lighting.glsl"
layout(binding = 1) uniform sampler2D gPosition;
layout(binding = 2) uniform sampler2D gNormal;
layout(binding = 3) uniform sampler2D gAlbedo;
layout(binding = 4) uniform sampler2D gMetallicRoughnessAO;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform UniformBufferShaderVariables {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 screenSize;
    vec3 cameraPos;
} ubo;

void main()
{
    vec4 baseColor = texture(gAlbedo, fragTexCoord);
    vec4 positionInput = texture(gPosition, fragTexCoord);
    vec4 normalWS = texture(gNormal, fragTexCoord);
    
    vec3 positionWS = positionInput.xyz;

    float lightIntensity = 5.0f;
    if (dot(normalWS.xyz, normalWS.xyz) < 1e-8) 
    {
        discard;
    }
    vec3 metallicRoughnessAO = texture(gMetallicRoughnessAO, fragTexCoord).rgb;
    float AO = metallicRoughnessAO.r;
    
    // reserve the normal in world space
    vec3 normal = normalize(normalWS.xyz * 2.0 - 1.0);

    float ambientIntensity = 0.3 * lightIntensity;

    // light properties
    vec3 lightColor = vec3(1.0, 0.75, 0.5) * lightIntensity;
    vec3 L = normalize(vec3(-1.0, 0.5, 1));  
    vec3 radiance = lightColor;  

    vec3 lightDir = normalize(L);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 viewDir = normalize(ubo.cameraPos - positionWS);

    vec3 directLight = PBR_Lighting_Direct(normal.xyz, viewDir, lightDir, baseColor.xyz, metallicRoughnessAO.x, metallicRoughnessAO.y, radiance);
    vec3 ambient = baseColor.rgb * AO * ambientIntensity;
    vec3 finalColor = directLight + ambient;

    outColor = vec4(finalColor, 1.0);
    //outColor *= PBRLighting(normal, lightDir);
}