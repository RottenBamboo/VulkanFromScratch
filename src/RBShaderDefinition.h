//
// Created by rottenbamboo on 2026/6/2.
//

#pragma once

#include <array>
#include <string>
#include <vector>
#include "RBMetaFile.h"
#include "json.h"

namespace RottenBamboo
{    
    enum class RBShaderParamType
    {
        Float,
        Vec2,
        Vec3,
        Vec4,
        Int,
        Bool,
        Texture2D,
        Sampler,
    };

    enum class RBShaderStageKind
    {
        Vertex,
        Fragment,
        Compute,
    };

    enum class RBShaderTextureType
    {
        Albedo,
        Normal,
        Cubemap,
        Custom,
    };

    struct RBShaderStageFile
    {
        RBShaderStageKind stage = RBShaderStageKind::Vertex;
        std::string path;
    };

    struct RBShaderParameter
    {
        std::string name;
        RBShaderParamType type = RBShaderParamType::Float;
        RBShaderTextureType textureType = RBShaderTextureType::Albedo;
        uint32_t set = 0;
        uint32_t binding = 0;
        std::vector<float> defaultFloatValues;
        int defaultIntValue = 0;
        bool defaultBoolValue = false;
    };

    struct RBShaderDefinitionData : public RBData
    {
        std::string name = "New Shader";
        std::vector<RBShaderStageFile> stages;
        std::vector<RBShaderParameter> parameters;
    };

    class RBShaderDefinition
    {
    public:
        RBShaderDefinition() = default;
        explicit RBShaderDefinition(const std::string& path);

        bool Load(const std::string& path);
        bool Save(const std::string& path = "") const;

        const RBShaderDefinitionData& GetData() const;
        RBShaderDefinitionData& GetData();

        void Reset();

        static RBShaderParamType ParseParamType(const std::string& typeName);
        static RBShaderTextureType ParseParamTextureType(const std::string& typeName);
        static std::string ToString(RBShaderParamType type);
        static std::string ToString(RBShaderTextureType type);
        static RBShaderStageKind ParseStageKind(const std::string& stageName);
        static std::string ToString(RBShaderStageKind stageKind);
    private:
        std::string path;
        RBShaderDefinitionData data;
        RBMetaFile metaFile;

        static void FillDefaultValues(const nlohmann::json& valueJson, RBShaderParameter& parameter);
        static nlohmann::json ParameterToJson(const RBShaderParameter& parameter);
        static nlohmann::json StageToJson(const RBShaderStageFile& stageFile);
    };
}