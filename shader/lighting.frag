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
    vec4 baseColor = texture(gAlbedo, fragTexCoord);
    vec4 positionInput = texture(gPosition, fragTexCoord);
    vec4 normalWS = texture(gNormal, fragTexCoord);
    vec3 positionWS = positionInput.xyz;
    float AOIntensity = positionInput.a;
    vec3 ambientLight = vec3(0.2);
    ambientLight *= AOIntensity;
    outColor.xyz =baseColor.rgb;
    //outColor.xyz += texture(gMaterial, fragTexCoord).xyz;

    float positionIsExist = dot(positionInput.xyz, vec3(1, 1, 1));
    outColor.xyz += ambientLight * (positionIsExist > 0 ? 1 : 0);
    //outColor.xyz += normalWS.xyz;
    outColor.a = 1.0f;
}