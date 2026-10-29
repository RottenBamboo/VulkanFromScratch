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

//standard GGX
float GGXStandard(float a2, float NdotH)
{
    float d = (NdotH * NdotH * (a2 - 1) + 1.0);
    return a2 / (PI * d * d);
}

//standard F
vec3 FresnelSchlickStandard(vec3 F0, float VoH)
{
    float Fc = pow(clamp(1.0 - VoH, 0.0, 1.0), 5.0);
    return F0 + (1.0 - F0) * Fc;
}

float GeometrySchlickGGX(float NdotX, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    return NdotX / clamp(NdotX * (1.0 - k) + k, 0.0001, 1.0);
}

//standard G
float GeometrySmithStandard(float NdotV, float NdotL, float roughness) {
    return GeometrySchlickGGX(NdotV, roughness) * GeometrySchlickGGX(NdotL, roughness);
}

//diffuse
vec3 Diffuse_Lambert(vec3 DiffuseColor)
{
    return DiffuseColor / PI;
}

//standard
vec3 Cook_Torrance(vec3 N, vec3 V, vec3 L, vec3 albedo, float metallic, float roughness)
{
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    vec3 H = normalize(L + V);
    float NdotH = clamp(dot(N, H), 0.0, 1.0);
    float NdotV = clamp(dot(N, V), 0.0, 1.0);
    float NdotL = clamp(dot(N, L), 0.0, 1.0);
    float VoH = clamp(dot(V, H), 0.0, 1.0);
    //specular
    float D = GGXStandard(alpha2, NdotH);
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 F = FresnelSchlickStandard(F0, VoH);
    float G = GeometrySmithStandard(NdotV, NdotL, roughness);
    float denom = max(4.0 * NdotV * NdotL, 0.0001);
    vec3 specular = (D * F * G / denom);

    //diffuse
    vec3 kS = 1.0 - F;
    vec3 kD = (1 - kS) * (1 - metallic);
    vec3 Diffuse = kD * Diffuse_Lambert(albedo);
    vec3 brdf = specular + Diffuse;
    return brdf;
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
    float a = Roughness * Roughness;
    float a2 = a * a;
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

vec3 PBR_Lighting_Direct_Standard(vec3 N, vec3 V, vec3 L, vec3 Albedo, float  Metallic, float  Roughness, vec3 Radiance)
{
    float NoL = clamp(dot(N, L), 0.0001, 1.0);
    return Cook_Torrance(N, V, L, Albedo, Metallic, Roughness) * Radiance * NoL;
}
