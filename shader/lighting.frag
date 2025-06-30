#version 450
layout(binding = 1) uniform sampler2D gPosition;
layout(binding = 2) uniform sampler2D gNormal;
layout(binding = 3) uniform sampler2D gAlbedo;
layout(binding = 4) uniform sampler2D gMetallicRoughnessAO;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main()
{
    vec4 baseColor = texture(gAlbedo, fragTexCoord);
    vec4 positionInput = texture(gPosition, fragTexCoord);
    vec4 normalWS = texture(gNormal, fragTexCoord);
    
    vec3 positionWS = positionInput.xyz;
    vec3 metallicRoughnessAO = texture(gMetallicRoughnessAO, fragTexCoord).rgb;
    float AOIntensity = metallicRoughnessAO.r;
    
    // reserve the normal in world space
    vec3 normal = normalize(normalWS.xyz * 2.0 - 1.0);

    vec3 ambientLight = vec3(0.3) * AOIntensity;

    // light properties
    vec3 lightPos = vec3(10.0, 10.0, 10.0);
    vec3 lightColor = vec3(1.0);

    vec3 lightDir = normalize(lightPos - positionWS);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 diffuse = diff * lightColor * baseColor.rgb;
    vec3 color = diffuse;

    bool positionIsExist = dot(normalWS.xyz, vec3(1.0)) <= 0.0f;

    if (positionIsExist) {
        discard;
    }
    else
    {
        color.xyz += ambientLight;
    }
    outColor = vec4(color, 1.0);
}