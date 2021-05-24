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
    , m_scene(scene)
    , m_componentsDeserializer(m_assetsIdRedirections) {
}

void Deserializer::deserialize(const std::string& path, std::shared_ptr<core::FileSystem> fileSystem) {

    if (not fileSystem->isFile(path))
        throw DeserializationError { ErrorCode::FileDoesNotExist, "Could not find file: " + path };

    auto fileContent = fileSystem->readFile(path);

    try {
        auto json = core::parseJson(fileContent);

        if (not json.isMember("assets") || not json.isMember("scene"))
            throw DeserializationError { ErrorCode::ProjectJsonIsInvalid };

        m_scene->clear();
        m_assetManager.clear();

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
        auto oldId = cubemapDescription["id"].asString();

        m_assetsIdRedirections[oldId] = cubemap->getId();
        m_assetManager.add(cubemap, cubemapDescription["name"].asString());

        SL_INFO("Found cubemap redirection: {} -> {}", oldId, cubemap->getId());
    }

    auto& models = assetsJson["models"];

    for (auto& modelToLoad : models["paths"])
        m_assetManager.add(
            geom::ModelLoader::load(modelToLoad.asString())->meshes);

    for (auto& meshDescription : models["meshes"]) {
        auto name = meshDescription["name"].asString();
        auto oldId = meshDescription["id"].asString();
        auto mesh = m_assetManager.getMeshes().getByName(name);

        m_assetsIdRedirections[oldId] = mesh->getId();

        SL_INFO("Found mesh redirection: {} -> {}", oldId, mesh->getId());
    }

    auto& defaultAssets = assetsJson["default-assets"];

    auto& globalShaders = GLOBALS().shaders->shadersByName;

    for (auto& shaderDescription : defaultAssets["shaders"]) {
        auto name = shaderDescription["name"].asString();

        if (globalShaders.contains(name)) {
            auto oldId = shaderDescription["id"].asString();
            auto newId = globalShaders.at(name)->getId();
            m_assetsIdRedirections[oldId] = newId;

            SL_INFO("Found redirection for default shader: {} - {} -> {}", name, oldId, newId);
        }
    }

    auto& globalMeshes = GLOBALS().geom->meshes;

    for (auto& meshDescription : defaultAssets["meshes"]) {
        auto name = meshDescription["name"].asString();

        if (globalMeshes.contains(name)) {
            auto oldId = meshDescription["id"].asString();
            auto newId = globalMeshes.at(name)->getId();
            m_assetsIdRedirections[oldId] = newId;

            SL_INFO("Found redirection for default mesh: {} - {} -> {}", name, oldId, newId);
        }
    }
}

void Deserializer::deserializeScene(Json::Value& sceneJson) {
    SL_INFO("Deserializing scene");

    if (sceneJson.isMember("skybox-id")) {
        auto skyboxOldId = sceneJson["skybox-id"].asString();

        auto cubemap = m_assetManager.getCubemaps().getById(
            m_assetsIdRedirections.at(skyboxOldId));
        auto skybox = scene::Skybox::create(GLOBALS().shaders->defaultCubemapShader, cubemap);
        m_scene->skybox = skybox;

        SL_INFO("Setting skybox: {}", skybox->cubemap->getId());
    }

    SL_INFO("Deserializing entities");
    for (auto& entityDescription : sceneJson["entities"]) {
        auto name = entityDescription["name"].asString();
        auto entity = m_scene->addEntity(name);

        SL_INFO("Deserializing entity: {}", name);
        SL_INFO("Deserializing components");

        for (auto& componentDescription : entityDescription["components"]) {
            auto componentName = componentDescription["name"].asString();
            SL_INFO("Deserializing {}", componentName);

            m_componentsDeserializer.deserializeComponent(componentName,
                componentDescription, *entity, m_assetManager);
        }
    }
}
}
