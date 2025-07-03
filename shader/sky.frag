#version 450
layout(binding = 1) uniform sampler2D albedoMap;

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


vec3 RotationUp(vec3 p, vec2 cos_sin)
{
    vec3 rotDirX = vec3(cos_sin.x, 0, -cos_sin.y);
    vec3 rotDirY = vec3(cos_sin.y, 0,  cos_sin.x);

    return vec3(dot(rotDirX, p), p.y, dot(rotDirY, p));
}

// vec4 GetColorWithRotation(vec3 dir, float Intensity, float exposure, vec2 cos_sin)
// {
//     dir = RotationUp(dir, cos_sin);
//     vec3 skyColor = texture(albedoMap, dir).xyz * Intensity * exposure;
//     //skyColor = ClampToFloat16Max(skyColor);
//     return vec4(skyColor, 1.0);
// }

// vec3 GetSkyViewDirWS(vec2 positionCS)
// {
//     vec4 viewDirWS = mul(vec4(positionCS.xy, 1.0f, 1.0f), _PixelCoordToViewDirWS);
//     return normalize(viewDirWS.xyz);
// }

void main()
{
    vec2 uv = (gl_FragCoord.xy / ubo.screenSize.xy) * 2.0 - 1.0;
    vec4 clipPos = vec4(uv, 1.0, 1.0); // z=1 表示远平面

    vec4 viewDir = inverse(ubo.proj) * clipPos;
    vec4 viewDirWS4 = inverse(ubo.view) * vec4(viewDir.xyz, 0.0);
    vec3 worldDirWS = -normalize(viewDirWS4.xyz);
    //outColor = GetColorWithRotation(worldDirWS, 1.0, 1.0, vec2(1.0));
    //outColor = vec4(worldDirWS.xyz, 1);
    //outColor = vec4(worldDirWS, 1);
    outColor.xyz = texture(albedoMap, worldDirWS.rg).xyz;
    outColor.a = 1.0f;
    //outColor = skyColor;
}