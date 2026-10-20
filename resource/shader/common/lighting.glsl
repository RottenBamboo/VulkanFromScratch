//
// Created by rottenbamboo on 2026/10/21.
//
#extension GL_GOOGLE_include_directive : require
#include "lightingUtility.glsl"

//d
float D_GGX(float a2, float NoH)
{
    float d = (NoH * a2 - NoH) * NoH + 1.0;
    return a2 / (PI * d * d);
}

//g
float Vis_SmithJointApprox(float a2, float NoV, float NoL)
{
    float a = sqrt(a2);
    float Vis_SmithV = NoL * (NoV * (1.0 - a) + a);
    float Vis_SmithL = NoV * (NoL * (1.0 - a) + a);
    return 0.5 * (1.0 / (Vis_SmithV + Vis_SmithL));
}

//f
vec3 F_Schlick(vec3 SpecularColor, float VoH)
{
    float Fc = pow(1.0 - VoH, 5.0);
    return clamp(50.0 * SpecularColor.g, 0.0, 1.0) * Fc + (1.0 - Fc) * SpecularColor;
}

//diffuse
vec3 Diffuse_Lambert(vec3 DiffuseColor)
{
    return DiffuseColor / PI;
}

vec3 PBR_Lighting(vec3 N, vec3 V, vec3 L, vec3 Albedo, float  Metallic, float  Roughness)
{
    vec3 H = normalize(L + V);
    float NoH = clamp(dot(N, H), 0.0, 1.0);
    float NoV = clamp(dot(N, V), 0.0, 1.0);
    float NoL = clamp(dot(N, L), 0.0, 1.0);
    float VoH = clamp(dot(V, H), 0.0, 1.0);

    if (NoL <= 0.0) return vec3(0.0);

    //vec3 F0 = vec3(0.04);
    vec3 F0 = mix(vec3(0.04), Albedo, Metallic);
    vec3 F = F_Schlick(F0, VoH);
    float a2 = Roughness * Roughness;
    float D = D_GGX(a2, NoH);
    float Vis = Vis_SmithJointApprox(a2, NoV, NoL);
    vec3 Specular = D * Vis * F;
    vec3 DiffuseColor = Albedo * (1.0 - Metallic);
    vec3 Diffuse = Diffuse_Lambert(DiffuseColor);
    vec3 BRDF = Diffuse + Specular;
    return BRDF;
}

vec3 PBR_Lighting_Direct(vec3 N, vec3 V, vec3 L, vec3 Albedo, float  Metallic, float  Roughness, vec3 Radiance)
{
    float NoL = clamp(dot(N, L), 0.0, 1.0);
    return PBR_Lighting(N, V, L, Albedo, Metallic, Roughness) * Radiance * NoL;
}
