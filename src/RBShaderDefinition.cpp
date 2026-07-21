//
// Created by rottenbamboo on 2026/6/2.
//

#include "RBShaderDefinition.h"

#include <fstream>
#include <sstream>
#include <filesystem>

#include "RBCommon.h"

namespace RottenBamboo
{
    namespace
    {
        bool ReadTextFile(const std::string& filePath, std::string& outText)
        {
            std::ifstream file(filePath, std::ios::in | std::ios::binary);
            if (!file.is_open())
            {
                return false;
            }

            std::stringstream buffer;
            buffer << file.rdbuf();
            outText = buffer.str();
            return true;
        }

        bool WriteTextFile(const std::string& filePath, const std::string& text)
        {
            std::ofstream file(filePath, std::ios::out | std::ios::trunc | std::ios::binary);
            if (!file.is_open())
            {
                return false;
            }

            file << text;
            return true;
        }
    }

    RBShaderDefinition::RBShaderDefinition(const std::string& path)
    {
        Load(path);
    }

    void RBShaderDefinition::Reset()
    {
        data = RBShaderDefinitionData{};
    }

    bool RBShaderDefinition::Load(const std::string& filePath)
    {
        if (!filePath.empty())
        {
            path = filePath;
        }

        Reset();

        if (path.empty())
        {
            return false;
        }

        const std::string absolutePath = std::filesystem::path(GET_PROJECT_ROOT_DIR).string() + "/" + path;
        std::string text;
        if (!ReadTextFile(absolutePath, text))
        {
            return false;
        }

        try
        {
            const auto jsonData = nlohmann::json::parse(text);

            if (jsonData.contains("name"))
            {
                data.name = jsonData["name"].get<std::string>();
            }

            if (jsonData.contains("stages") && jsonData["stages"].is_array())
            {
                for (const auto& stageJson : jsonData["stages"])
                {
                    RBShaderStageFile stageFile{};
                    if (stageJson.contains("stage"))
                    {
                        stageFile.stage = ParseStageKind(stageJson["stage"].get<std::string>());
                    }
                    if (stageJson.contains("path"))
                    {
                        stageFile.path = NormalizePathString(stageJson["path"].get<std::string>());
                    }
                    data.stages.push_back(stageFile);
                }
            }

            if (jsonData.contains("parameters") && jsonData["parameters"].is_array())
            {
                for (const auto& parameterJson : jsonData["parameters"])
                {
                    RBShaderParameter parameter{};
                    if (parameterJson.contains("name"))
                    {
                        parameter.name = parameterJson["name"].get<std::string>();
                    }
                    if (parameterJson.contains("type"))
                    {
                        parameter.type = ParseParamType(parameterJson["type"].get<std::string>());
                    }
                    if (parameterJson.contains("textureType"))
                    {
                        parameter.textureType = ParseParamTextureType(parameterJson["textureType"].get<std::string>());
                    }
                    if (parameterJson.contains("set"))
                    {
                        parameter.set = parameterJson["set"].get<uint32_t>();
                    }
                    if (parameterJson.contains("binding"))
                    {
                        parameter.binding = parameterJson["binding"].get<uint32_t>();
                    }
                    if (parameterJson.contains("default"))
                    {
                        FillDefaultValues(parameterJson["default"], parameter);
                    }
                    data.parameters.push_back(parameter);
                }
            }

            return true;
        }
        catch (...)
        {
            Reset();
            return false;
        }
    }

    bool RBShaderDefinition::Save(const std::string& filePath) const
    {
        const std::string targetPath = filePath.empty() ? path : filePath;
        if (targetPath.empty())
        {
            return false;
        }

        std::filesystem::path absolutePath = targetPath;
        if (absolutePath.is_relative())
        {
            absolutePath = std::filesystem::path(GET_PROJECT_ROOT_DIR) / absolutePath;
        }

        if (absolutePath.has_parent_path())
        {
            std::filesystem::create_directories(absolutePath.parent_path());
        }

        nlohmann::json jsonData;
        jsonData["name"] = data.name;
        jsonData["stages"] = nlohmann::json::array();
        jsonData["parameters"] = nlohmann::json::array();

        for (const auto& stageFile : data.stages)
        {
            jsonData["stages"].push_back(StageToJson(stageFile));
        }

        for (const auto& parameter : data.parameters)
        {
            jsonData["parameters"].push_back(ParameterToJson(parameter));
        }

        return WriteTextFile(absolutePath.string(), jsonData.dump(4));
    }

    const RBShaderDefinitionData& RBShaderDefinition::GetData() const
    {
        return data;
    }

    RBShaderDefinitionData& RBShaderDefinition::GetData()
    {
        return data;
    }

    RBShaderParamType RBShaderDefinition::ParseParamType(const std::string& typeName)
    {
        if (typeName == "vec2") return RBShaderParamType::Vec2;
        if (typeName == "vec3") return RBShaderParamType::Vec3;
        if (typeName == "vec4") return RBShaderParamType::Vec4;
        if (typeName == "int") return RBShaderParamType::Int;
        if (typeName == "bool") return RBShaderParamType::Bool;
        if (typeName == "texture2D") return RBShaderParamType::Texture2D;
        if (typeName == "sampler") return RBShaderParamType::Sampler;
        return RBShaderParamType::Float;
    }

    RBShaderTextureType RBShaderDefinition::ParseParamTextureType(const std::string& typeName)
    {
        if (typeName == "Albedo") return RBShaderTextureType::Albedo;
        if (typeName == "Normal") return RBShaderTextureType::Normal;
        if (typeName == "Cubemap") return RBShaderTextureType::Cubemap;
        if (typeName == "Custom") return RBShaderTextureType::Custom;
        return RBShaderTextureType::Albedo;
    }
    std::string RBShaderDefinition::ToString(RBShaderParamType type)
    {
        switch (type)
        {
            case RBShaderParamType::Float: return "float";
            case RBShaderParamType::Vec2: return "vec2";
            case RBShaderParamType::Vec3: return "vec3";
            case RBShaderParamType::Vec4: return "vec4";
            case RBShaderParamType::Int: return "int";
            case RBShaderParamType::Bool: return "bool";
            case RBShaderParamType::Texture2D: return "texture2D";
            case RBShaderParamType::Sampler: return "sampler";
            default: return "float";
        }
    }

    std::string RBShaderDefinition::ToString(RBShaderTextureType type)
    {
        switch (type)
        {
            case RBShaderTextureType::Albedo: return "Albedo";
            case RBShaderTextureType::Normal: return "Normal";
            case RBShaderTextureType::Cubemap: return "Cubemap";
            case RBShaderTextureType::Custom: return "Custom";
            default: return "Albedo";
        }
    }

    RBShaderStageKind RBShaderDefinition::ParseStageKind(const std::string& stageName)
    {
        if (stageName == "fragment" || stageName == "frag") return RBShaderStageKind::Fragment;
        if (stageName == "compute") return RBShaderStageKind::Compute;
        return RBShaderStageKind::Vertex;
    }

    std::string RBShaderDefinition::ToString(RBShaderStageKind stageKind)
    {
        switch (stageKind)
        {
        case RBShaderStageKind::Vertex: return "vertex";
        case RBShaderStageKind::Fragment: return "fragment";
        case RBShaderStageKind::Compute: return "compute";
        default: return "vertex";
        }
    }

    void RBShaderDefinition::FillDefaultValues(const nlohmann::json& valueJson, RBShaderParameter& parameter)
    {
        if (valueJson.is_boolean())
        {
            parameter.defaultBoolValue = valueJson.get<bool>();
            return;
        }

        if (valueJson.is_number_integer())
        {
            parameter.defaultIntValue = valueJson.get<int>();
            return;
        }

        if (valueJson.is_number())
        {
            parameter.defaultFloatValues = { valueJson.get<float>() };
            return;
        }

        if (valueJson.is_array())
        {
            parameter.defaultFloatValues.clear();
            for (const auto& item : valueJson)
            {
                parameter.defaultFloatValues.push_back(item.get<float>());
            }
        }
    }

    nlohmann::json RBShaderDefinition::ParameterToJson(const RBShaderParameter& parameter)
    {
        nlohmann::json jsonData;
        jsonData["name"] = parameter.name;
        jsonData["type"] = ToString(parameter.type);
        jsonData["textureType"] = ToString(parameter.textureType);
        jsonData["set"] = parameter.set;
        jsonData["binding"] = parameter.binding;

        switch (parameter.type)
        {
        case RBShaderParamType::Bool:
            jsonData["default"] = parameter.defaultBoolValue;
            break;
        case RBShaderParamType::Int:
            jsonData["default"] = parameter.defaultIntValue;
            break;
        case RBShaderParamType::Texture2D:
        case RBShaderParamType::Sampler:
            break;
        default:
            jsonData["default"] = parameter.defaultFloatValues;
            break;
        }

        return jsonData;
    }

    nlohmann::json RBShaderDefinition::StageToJson(const RBShaderStageFile& stageFile)
    {
        nlohmann::json jsonData;
        jsonData["stage"] = ToString(stageFile.stage);
        jsonData["path"] = stageFile.path;
        return jsonData;
    }
}