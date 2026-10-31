//
// Created by rottenbamboo on 2026/9/4.
//
#include "RBScene.h"
using nlohmann::json;
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
        m_Root = std::make_unique<RBSceneNode>("Root", nullptr, uuids::uuid());
        return;
    }

    path = targetPath;
    std::filesystem::path filePath(GET_RESOURCE_ROOT_DIR + sceneFilePath + targetPath);
    RBLOG_INFO("RBScene::Load() loadPath : " + filePath.string());

    if (!std::filesystem::exists(filePath))
    {
        RBLOG_INFO("RBScene::Load() scene file does not exist, create empty root : " + filePath.string());
        m_Root = std::make_unique<RBSceneNode>("Root", nullptr, uuids::uuid());
        return;
    }

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        RBLOG_INFO("RBScene::Load() cannot open file : " + filePath.string());
        m_Root = std::make_unique<RBSceneNode>("Root", nullptr, uuids::uuid());
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    const std::string sceneText = buffer.str();

    m_Root = std::make_unique<RBSceneNode>("Root", nullptr, uuids::uuid());
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
    data.name = j.value("SceneName", std::string("Scene"));

    // node data
    if (!j.contains("nodes") || !j["nodes"].is_array())
    {
        RBLOG_INFO("RBScene::Load() no \"nodes\" array, skip node loading");
    }
    else
    {
        for (const auto& jn : j["nodes"])
        {
            if (!jn.is_object()) continue;

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

            RBSceneNode* node = m_Root->CreateChild(nodeName);
            if (!node) continue;

            node->m_Transform = Transform(pos, scale, quat(rot));
            node->m_Transform.SetEuler(rot);

            RBSceneNodeData nodeData;
            nodeData.name     = nodeName;
            nodeData.position = pos;
            nodeData.rotate   = rot;
            nodeData.scale    = scale;
            data.nodes.push_back(nodeData);
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

        std::string sceneFileData = "{\n";
        sceneFileData += "    \"SceneName\": \"" + sceneData.name + "\",\n";
        sceneFileData += "    \"nodes\": [";

        bool firstNode = true;

        std::function<void(const RBSceneNode*)> traverseNode = [&](const RBSceneNode* node) 
        {
            for (auto& child : node->m_Children)
            {
                if (child) 
                {
                    sceneFileData += firstNode ? "\n" : ",\n";
                    firstNode = false;

                    sceneFileData += "        {\n";
                    sceneFileData += "            \"name\": \"" + child->GetName() + "\",\n";
                    sceneFileData += "            \"position\": [";
                    sceneFileData += std::to_string((float)child->m_Transform.GetPosition().x) + ",";
                    sceneFileData += std::to_string((float)child->m_Transform.GetPosition().y) + ",";
                    sceneFileData += std::to_string((float)child->m_Transform.GetPosition().z);
                    sceneFileData += "],\n";
                    sceneFileData += "            \"rotation\": [";
                    sceneFileData += std::to_string((float)child->m_Transform.GetRotation().x) + ",";
                    sceneFileData += std::to_string((float)child->m_Transform.GetRotation().y) + ",";
                    sceneFileData += std::to_string((float)child->m_Transform.GetRotation().z);
                    sceneFileData += "],\n";
                    sceneFileData += "            \"scale\": [";
                    sceneFileData += std::to_string((float)child->m_Transform.GetScale().x) + ",";
                    sceneFileData += std::to_string((float)child->m_Transform.GetScale().y) + ",";
                    sceneFileData += std::to_string((float)child->m_Transform.GetScale().z);
                    sceneFileData += "]\n";
                    sceneFileData += "        }";

                    traverseNode(child.get());
                }
            }
        };
        traverseNode(root);

        sceneFileData += firstNode ? "]\n" : "\n    ]\n";
        sceneFileData += "}\n";
        file << sceneFileData;
        RBLOG_INFO("RBScene::Save() scene = " + sceneFileData);
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