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

    //             +Y (theta = π/2, v=1)
    //              |
    //              |
    //  (-π,1)       |        (0,1)      (π,1)
    // ------------ O -------------> +X
    //             /
    //          Z /
    //         (-π,0)     (π,0)

    // θ = asin(y)
    // φ = atan(z, x)

vec2 dirToEquirectUV(vec3 dir)
{
    float phi = atan(dir.z, dir.x);     // [-π, π]
    float theta = asin(clamp(dir.y, -1.0, 1.0)); // [-π/2, π/2]

    float u = 1.0 - (phi + 3.1415926) / (2.0 * 3.1415926); // [0,1]
    float v = (theta + 3.1415926 / 2.0) / 3.1415926;       // [0,1]

    return vec2(u, v);
}
float ST2084_encode_scalar(float L)
{
    float m1 = 0.1593017578125;
    float m2 = 78.84375;
    float c1 = 0.8359375;
    float c2 = 18.8515625;
    float c3 = 18.6875;

    float Lm1 = pow(L, m1);
    return pow((c1 + c2 * Lm1) / (1.0 + c3 * Lm1), m2);
}

vec3 ST2084_encode(vec3 color)
{
    return vec3(
        ST2084_encode_scalar(color.r),
        ST2084_encode_scalar(color.g),
        ST2084_encode_scalar(color.b)
    );
}
void main()
{
    vec2 uv = (gl_FragCoord.xy / ubo.screenSize.xy) * 2.0 - 1.0;
    vec4 clipPos = vec4(uv, 1.0, 1.0); // z=1 表示远平面

    vec4 viewDir = inverse(ubo.proj) * clipPos;
    vec4 viewDirWS4 = inverse(ubo.view) * vec4(viewDir.xyz, 0.0);
    vec3 worldDirWS = -normalize(viewDirWS4.xyz);

    vec2 envUV = dirToEquirectUV(worldDirWS);
    //outColor = GetColorWithRotation(worldDirWS, 1.0, 1.0, vec2(1.0));
    //outColor = vec4(worldDirWS.xyz, 1);
    //outColor = vec4(worldDirWS, 1);
    vec3 hdrColor = texture(albedoMap, envUV).xyz;
    // ✅ 1. Tone Mapping（例如 Reinhard/ACES）
    vec3 mappedColor = hdrColor / (hdrColor + vec3(1.0)); // Reinhard

    // ✅ 2. Gamma Correction（因为是 UNORM 输出）
    vec3 finalColor = pow(mappedColor, vec3(1.0 / 2.2));

    mappedColor = ST2084_encode(mappedColor); // custom function
    // 输出到 SDR framebuffer
    outColor = vec4(hdrColor, 1.0);
    //outColor = skyColor;
}