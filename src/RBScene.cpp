//
// Created by rottenbamboo on 2026/9/4.
//
#include "RBScene.h"
#include "RBApplication.h"
using nlohmann::json;
namespace fs = std::filesystem;
namespace RottenBamboo
{
    RBScene::RBScene(const std::string &path, const std::string& nodeName, const uuids::uuid& guid) 
     : RBResource(path), m_Root(std::make_unique<RBSceneNode>(nodeName, nullptr, guid)) 
    {

    }

    void RBScene::Load(const std::string& loadPath) 
    {
        std::string targetPath = loadPath;
        if (targetPath.empty())
        {
            RBLOG_INFO("RBScene::Load() loadPath is empty. Set to default path : " + path);
            targetPath = path;
        }

        if (targetPath.empty())
        {
            RBLOG_INFO("RBScene::Load() loadPath : " + loadPath);
            m_Root = std::make_unique<RBSceneNode>("untitled", nullptr, uuids::uuid());
            return;
        }

        path = targetPath;
        std::filesystem::path filePath(GET_RESOURCE_ROOT_DIR + sceneFilePath + targetPath);
        RBLOG_INFO("RBScene::Load() loadPath : " + filePath.string());

        if (!std::filesystem::exists(filePath))
        {
            RBLOG_INFO("RBScene::Load() scene file does not exist, create empty root : " + filePath.string());
            m_Root = std::make_unique<RBSceneNode>("untitled", nullptr, uuids::uuid());
            return;
        }

        std::ifstream file(filePath);
        if (!file.is_open())
        {
            RBLOG_INFO("RBScene::Load() cannot open file : " + filePath.string());
            m_Root = std::make_unique<RBSceneNode>("untitled", nullptr, uuids::uuid());
            return;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        const std::string sceneText = buffer.str();

        m_Root = std::make_unique<RBSceneNode>("untitled", nullptr, uuids::uuid());
        data = RBSceneData{};

        // parse jason file
        json j;
        try
        {
            j = json::parse(sceneText);
        }
        catch (const std::exception& e)
        {
            RBLOG_INFO(std::string("RBScene::Load() JSON parse failed: ") + e.what());
            return;   // m_Root empty
        }

        // scene name
        data.name = j.value("SceneName", std::string());
        m_Root->SetName(data.name);
        // node data
        if (!j.contains("nodes") || !j["nodes"].is_array())
        {
            RBLOG_INFO("RBScene::Load() no \"nodes\" array, skip node loading");
        }
        else
        {
            std::function<void(const json&, RBSceneNode*)> parseNode = [&](const json& jn, RBSceneNode* parent)
            {
                if (!jn.is_object()) return;

                std::string nodeName = jn.value("name", std::string());

                auto readVec3 = [&jn](const char* key) -> vector3
                {
                    if (!jn.contains(key) || !jn[key].is_array() || jn[key].size() < 3)
                        return vector3(0, 0, 0);
                    return vector3(
                        jn[key][0].get<float>(),
                        jn[key][1].get<float>(),
                        jn[key][2].get<float>()
                    );
                };

                vector3 pos   = readVec3("position");
                vector3 rot   = readVec3("rotation");
                vector3 scale = readVec3("scale");
                
                //load model guid
                uuids::uuid modelGuid{};
                std::string modelGuidStr = jn.value("modelGUID", std::string());
                if (!modelGuidStr.empty())
                {
                    auto optGuid = uuids::uuid::from_string(modelGuidStr);
                    if (optGuid)
                    {
                        modelGuid = *optGuid;
                    }
                    else
                    {
                        RBLOG_INFO("RBScene::Load() invalid modelGUID: " + modelGuidStr
                                   + " for node: " + nodeName);
                    }
                }
                auto modelPathOpt = RBApplication::GetAssetRegistry()->GetPath(modelGuid);
                std::string modelPath = modelPathOpt.has_value() ? modelPathOpt.value() : "";
                modelPath = GET_RESOURCE_ROOT_DIR + modelPath;

                //load material guid
                uuids::uuid materialGuid{};
                std::string materialGuidStr = jn.value("materialGUID", std::string());
                if (!materialGuidStr.empty())
                {
                    auto optGuid = uuids::uuid::from_string(materialGuidStr);
                    if (optGuid)
                    {
                        materialGuid = *optGuid;
                    }
                    else
                    {
                        RBLOG_INFO("RBScene::Load() invalid materialGUID: " + materialGuidStr
                                   + " for node: " + nodeName);
                    }
                }
                auto materialPathOpt = RBApplication::GetAssetRegistry()->GetPath(materialGuid);
                std::string materialPath = materialPathOpt.has_value() ? materialPathOpt.value() : "";

                //load descriptor offset
                std::string descriptorOffsetStr = jn.value("descriptorOffset", std::string());
                int descriptorOffset = 0;
                if (!descriptorOffsetStr.empty())
                {
                    try
                    {
                        descriptorOffset = static_cast<int>(std::stof(descriptorOffsetStr));
                    }
                    catch (const std::exception&)
                    {
                        RBLOG_INFO("RBScene::Load() invalid descriptorOffset: " + descriptorOffsetStr
                                   + " for node: " + nodeName);
                        descriptorOffset = 0;
                    }
                }

                RBSceneNode* node = parent ? parent->CreateChild(nodeName) : m_Root->CreateChild(nodeName);
                if (!node) return;

                ResourceManager* resourceManager = RBApplication::GetResourceManager();
                auto material = RBApplication::instance->AddMaterial(materialPath);
                auto model = modelPathOpt.has_value() ? resourceManager->Load<RBModel>(modelPath) : nullptr;

                node->m_Material = material;
                node->m_Model = model;
                node->m_Resource = model ? model.get() : nullptr;
                node->m_Transform = Transform(pos, scale, quat(rot));
                node->m_Transform.SetEuler(rot);
                node->SetDescriptorOffset(descriptorOffset);

                RBSceneNodeData nodeData;
                nodeData.name     = nodeName;
                nodeData.position = pos;
                nodeData.rotate   = rot;
                nodeData.scale    = scale;
                data.nodes.push_back(nodeData);

                if (jn.contains("nodes") && jn["nodes"].is_array())
                {
                    for (const auto& childJson : jn["nodes"])
                    {
                        parseNode(childJson, node);
                    }
                }
            };

            for (const auto& jn : j["nodes"])
            {
                parseNode(jn, nullptr);
            }
        }

        std::string metaPath = targetPath + META_EXTENSION;
        if (std::filesystem::exists(GET_RESOURCE_ROOT_DIR + metaPath))
        {
            metaFile.Load(metaPath, data);
        }
    }

    bool RBScene::Save(const std::string& savePath, RBSceneData& sceneData, const RBSceneNode* root)
    {
        std::string currentPath;
        if(savePath.empty())
        {
            RBLOG_INFO("RBScene::Save() savePath is empty. Set to default path : " + path);
            currentPath = path;
        }
        else
        {
            RBLOG_INFO("RBScene::Save() path : " + path);
            currentPath = savePath;
        }
    
        //save scene file
        std::string scenePath = currentPath + SCENE_EXTENSION;
        RBLOG_INFO("RBScene::Save() targetPath : " + scenePath);
        try 
        {
            std::filesystem::path sceneFilePath(GET_RESOURCE_ROOT_DIR + scenePath);
            RBLOG_INFO("RBScene::Save() sceneFilePath : " + sceneFilePath.string());
            if (sceneFilePath.has_parent_path()) 
            {
                std::filesystem::create_directories(sceneFilePath.parent_path());
                RBLOG_INFO("RBScene::Save() create directories : " + sceneFilePath.parent_path().string());
            }
        
            std::ofstream file(sceneFilePath, std::ios::trunc);
            if (!file.is_open()) 
            {
                RBLOG_INFO("RBScene::Save() cannot open file : " + sceneFilePath.string());
                return false;
            }

            const RBSceneNode* saveRoot = root != nullptr ? root : m_Root.get();
            std::function<void(const RBSceneNode*, std::ostream&, int)> appendNodeJson = [&](const RBSceneNode* node, std::ostream& out, int indent)
            {
                if (!node)
                {
                    return;
                }

                const std::string pad = std::string(indent, ' ');
                const std::string childPad = std::string(indent + 4, ' ');

                std::string modelGuidStr = "";
                if (node->m_Model && !node->m_Model->GetPath().empty())
                {
                    auto relativeModelPath = fs::relative(node->m_Model->GetPath(), GET_RESOURCE_ROOT_DIR).string();
                    auto modelGuid = RBApplication::GetAssetRegistry()->GetGuid(relativeModelPath);
                    if (modelGuid.has_value())
                    {
                        modelGuidStr = uuids::to_string(modelGuid.value());
                    }
                }

                std::string materialGuidStr = "";
                if (node->m_Material && !node->m_Material->GetPath().empty())
                {
                    auto materialGuid = RBApplication::GetAssetRegistry()->GetGuid(node->m_Material->GetPath());
                    if (materialGuid.has_value())
                    {
                        materialGuidStr = uuids::to_string(materialGuid.value());
                    }
                }

                out << pad << "{\n";
                out << childPad << "\"name\": \"" << node->GetName() << "\",\n";
                out << childPad << "\"position\": [" << std::to_string(node->m_Transform.GetPosition().x) << ", "
                    << std::to_string(node->m_Transform.GetPosition().y) << ", "
                    << std::to_string(node->m_Transform.GetPosition().z) << "],\n";
                out << childPad << "\"rotation\": [" << std::to_string(node->m_Transform.GetRotation().x) << ", "
                    << std::to_string(node->m_Transform.GetRotation().y) << ", "
                    << std::to_string(node->m_Transform.GetRotation().z) << "],\n";
                out << childPad << "\"scale\": [" << std::to_string(node->m_Transform.GetScale().x) << ", "
                    << std::to_string(node->m_Transform.GetScale().y) << ", "
                    << std::to_string(node->m_Transform.GetScale().z) << "],\n";
                out << childPad << "\"modelGUID\": \"" << modelGuidStr << "\",\n";
                out << childPad << "\"materialGUID\": \"" << materialGuidStr << "\",\n";
                out << childPad << "\"descriptorOffset\": \"" << std::to_string(node->GetDescriptorOffset()) << "\",\n";
                out << childPad << "\"nodes\": [";

                if (!node->m_Children.empty())
                {
                    out << "\n";
                    for (size_t i = 0; i < node->m_Children.size(); ++i)
                    {
                        if (!node->m_Children[i]) continue;
                        appendNodeJson(node->m_Children[i].get(), out, indent + 8);
                        if (i + 1 < node->m_Children.size())
                        {
                            out << ",";
                        }
                        out << "\n";
                    }
                    out << pad << "    ]\n";
                }
                else
                {
                    out << "]\n";
                }

                out << pad << "}";
            };

            std::ostringstream sceneFileData;
            sceneFileData << "{\n";
            sceneFileData << "    \"SceneName\": \"" << sceneData.name << "\",\n";
            sceneFileData << "    \"nodes\": [\n";

            if (saveRoot)
            {
                for (size_t i = 0; i < saveRoot->m_Children.size(); ++i)
                {
                    if (!saveRoot->m_Children[i]) continue;
                    appendNodeJson(saveRoot->m_Children[i].get(), sceneFileData, 8);
                    if (i + 1 < saveRoot->m_Children.size())
                    {
                        sceneFileData << ",";
                    }
                    sceneFileData << "\n";
                }
            }

            sceneFileData << "    ]\n";
            sceneFileData << "}\n";
            file << sceneFileData.str();
            RBLOG_INFO("RBScene::Save() scene = " + sceneFileData.str());
        }
        catch (const std::exception& e)
        {
            std::cout << "RBScene::Save() failed for " << scenePath << ": " << e.what() << std::endl;
            return false;
        }
    
        //save scene metadata file
        std::string matMetaPath = scenePath + META_EXTENSION;
        RBLOG_INFO("RBScene::Save() targetMetaPath : " + matMetaPath);
    
        if(!metaFile.Save(matMetaPath, sceneData))
        {
            RBLOG_INFO("RBScene::Save() failed to save meta file : " + matMetaPath);
            return false;
        }
    
        data.m_GUID = sceneData.m_GUID;
        return true;
    }

    RBSceneData& RBScene::GetData()
    {
        return data;
    }

    const RBSceneData& RBScene::GetData() const
    {
        return data;
    }

    void RBScene::SetRoot(std::unique_ptr<RBSceneNode> root)
    {
        m_Root = std::move(root);
    }

    RBSceneNode* RBScene::GetRoot()
    {
        return m_Root.get();
    }

    const RBSceneNode* RBScene::GetRoot() const
    {
        return m_Root.get();
    }
}