#version 450
layout(binding = 1) uniform sampler2D albedoMap;
layout(binding = 2) uniform sampler2D normalMap;
layout(binding = 3) uniform sampler2D AOMap;
layout(binding = 4) uniform sampler2D emissionMap;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 positionWS;
layout(location = 3) in vec3 fragNormal;
layout(location = 4) in vec3 fragTangent;
layout(location = 5) in vec3 fragBitangent;

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gAlbedo;
layout(location = 3) out vec4 gMaterial;

void main()
{
    float AOIntensity = texture(AOMap, fragTexCoord).r;
    gPosition = vec4(positionWS, AOIntensity);//texture(albedoMap, fragTexCoord);
    // tangentNormal = texture(normalMap, fragTexCoord).rgb;
    vec3 tangentNormal = texture(normalMap, fragTexCoord).rgb;
    tangentNormal = tangentNormal * 2.0 - 1.0;

    // construct TBN matrix
    mat3 TBN = mat3(normalize(fragTangent), normalize(fragBitangent), normalize(fragNormal));
    vec3 worldNormal = normalize(TBN * tangentNormal);

    // output the normal in world space
    gNormal = vec4(worldNormal * 0.5 + 0.5, 1.0);
    
    gAlbedo = texture(albedoMap, fragTexCoord);
    gMaterial = texture(emissionMap, fragTexCoord);
}