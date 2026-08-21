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

const float m1 = 0.1593017578125;
const float m2 = 78.84375;
const float c1 = 0.8359375;
const float c2 = 18.8515625;
const float c3 = 18.6875;

// Reinhard tone mapping
vec3 ToneMapReinhard(vec3 x) {
    return x / (x + vec3(1.0));
}

// Convert to PQ-encoded value
float LinearToPQ(float L)
{
    float Lp = pow(L / 1000.0, m1); // Assume target display max 1000 nits
    return pow((c1 + c2 * Lp) / (1.0 + c3 * Lp), m2);
}

vec3 EncodePQ(vec3 colorNits)
{
    return vec3(
        LinearToPQ(colorNits.r),
        LinearToPQ(colorNits.g),
        LinearToPQ(colorNits.b)
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
    vec3 hdrColor = texture(albedoMap, envUV).xyz;

    // vec3 toneMapped = hdrColor / (hdrColor + vec3(1.0));

    // // 2. Map to nits (1000 nits target)
    // vec3 nits = toneMapped * 1000.0;

    // // 3. Encode PQ
    // vec3 pqEncoded = EncodePQ(nits);

    outColor = vec4(hdrColor, 1.0);
    //outColor = skyColor;
}