#include "Deserializer.h"

#include "sl/core/Errors.hpp"
#include "sl/core/Json.h"
#include "sl/core/Logger.h"
#include "sl/geom/ModelLoader.hpp"
#include "sl/gfx/Cubemap.h"
#include "sl/gfx/Shader.h"
#include "sl/utils/Globals.h"

using namespace sl::core;

namespace sl::app {

Deserializer::Deserializer(asset::AssetManager& assetManager, std::shared_ptr<scene::Scene> scene)
    : m_assetManager(assetManager)
    , m_scene(scene) {
}

void Deserializer::deserialize(const std::string& path, std::shared_ptr<core::FileSystem> fileSystem) {
    if (not fileSystem->isFile(path))
        throw DeserializationError { ErrorCode::FileDoesNotExist, "Could not find file: " + path };

    auto fileContent = fileSystem->readFile(path);

    try {
        auto json = core::parseJson(fileContent);

        if (not json.isMember("assets") || not json.isMember("scene"))
            throw DeserializationError { ErrorCode::ProjectJsonIsInvalid };

        deserializeAssets(json["assets"]);
        deserializeScene(json["scene"]);

    } catch (JsonError& e) {
        throw DeserializationError { ErrorCode::InvalidJsonString, e.getDetails() };
    } catch (Json::Exception& e) {
        throw DeserializationError { ErrorCode::ProjectJsonIsInvalid };
    }
}

void Deserializer::deserializeAssets(Json::Value& assetsJson) {
    SL_INFO("Deserializing assets");

    for (auto& cubemapDescription : assetsJson["cubemaps"]) {
        auto& paths = cubemapDescription["paths"];
        gfx::CubemapArgs faces;

        for (int i = 0; i < faces.size(); ++i)
            faces[i] = paths[i].asString();

        auto cubemap = gfx::Cubemap::load(faces);
        cubemap->setId(cubemapDescription["id"].asInt());

        m_assetManager.add(cubemap, cubemapDescription["name"].asString());
    }

    auto& models = assetsJson["models"];

    for (auto& modelToLoad : models["paths"])
        m_assetManager.add(
            geom::ModelLoader::load(modelToLoad.asString())->meshes);

    for (auto& meshDescription : models["meshes"]) {
        auto name = meshDescription["name"].asString();
        m_assetManager.getMeshes().getByName(name)->setId(
            meshDescription["id"].asInt());
    }
}

void Deserializer::deserializeScene(Json::Value& sceneJson) {
    SL_INFO("Deserializing scene");

    if (sceneJson.isMember("skybox-id")) {
        auto skyboxId = sceneJson["skybox-id"].asUInt();
        auto cubemap = m_assetManager.getCubemaps().getById(skyboxId);
        auto skybox = scene::Skybox::create(GLOBALS().shaders->defaultCubemapShader, cubemap);
        m_scene->skybox = skybox;
        SL_INFO("Setting skybox: {}", skyboxId);
    }

    SL_INFO("Deserializing entities");
    for (auto& entityDescription : sceneJson["entities"]) {
        auto name = entityDescription["name"].asString();
        auto entity = m_scene->addEntity(name);

        SL_INFO("Deserializing entity: {}", name);
        SL_INFO("Deserializing components");

        for (auto& componentDescription : entityDescription["components"]) {
            auto componentName = componentDescription["name"].asString();

            m_componentsDeserializer.deserializeComponent(componentName,
                componentDescription, *entity, m_assetManager);
        }
    }
}
}
