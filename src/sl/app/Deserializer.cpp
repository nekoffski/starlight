#include "Deserializer.h"

#include <kc/core/Log.h>
#include <kc/json/Json.h>

#include "sl/core/Errors.hpp"
#include "sl/geom/GeometryManager.h"
#include "sl/gfx/Cubemap.h"
#include "sl/gfx/Shader.h"
#include "sl/gfx/TextureManager.h"
#include "sl/glob/Globals.h"

using namespace sl::core;

namespace sl::app {

Deserializer::Deserializer(asset::AssetManager& assetManager, std::shared_ptr<scene::Scene> scene)
    : m_assetManager(assetManager)
    , m_scene(scene)
    , m_componentsDeserializer(m_assetsIdRedirections) {
}

void Deserializer::deserialize(const std::string& path, const kc::core::FileSystem& fileSystem) {

    if (not fileSystem.isFile(path))
        throw DeserializationError { "Could not find file: " + path };

    auto fileContent = fileSystem.readFile(path);

    try {
        auto json = kc::json::loadJson(fileContent);

        if (not json.isMember("assets") || not json.isMember("scene"))
            throw DeserializationError {};

        m_scene->clear();
        m_assetManager.clear();

        deserializeAssets(json["assets"]);
        deserializeScene(json["scene"]);

    } catch (JsonError& e) {
        throw DeserializationError { e.getDetails() };
    } catch (Json::Exception& e) {
        throw DeserializationError { e.what() };
    }
}

void Deserializer::deserializeAssets(kc::json::Node& assetsJson) {
    LOG_INFO("Deserializing assets");

    for (auto& cubemapDescription : assetsJson["cubemaps"]) {
        auto& paths = cubemapDescription["paths"];
        gfx::CubemapArgs faces;

        for (int i = 0; i < faces.size(); ++i)
            faces[i] = paths[i].asString();

        auto cubemap = gfx::TextureManager::get().createCubemap(faces, cubemapDescription["name"].asString());
        auto oldId = cubemapDescription["id"].asString();
        auto newId = cubemap->getId();

        m_assetsIdRedirections[oldId] = newId;
        m_assetManager.add(std::move(cubemap));

        LOG_INFO("Found cubemap redirection: {} -> {}", oldId, newId);
    }

    for (auto& textureDescription : assetsJson["textures"]) {
        auto texture = gfx::TextureManager::get().createTexture(textureDescription["path"].asString(),
            textureDescription["name"].asString());

        auto oldId = textureDescription["id"].asString();
        auto newId = texture->getId();

        m_assetsIdRedirections[oldId] = newId;
        m_assetManager.add(std::move(texture));

        LOG_INFO("Found texture redirection: {} -> {}", oldId, newId);
    }

    auto& models = assetsJson["models"];

    for (auto& modelToLoad : models["paths"])
        m_assetManager.add(
            geom::GeometryManager::get().loadModel(modelToLoad.asString())->meshes);

    for (auto& meshDescription : models["meshes"]) {
        auto name = meshDescription["name"].asString();
        auto oldId = meshDescription["id"].asString();
        auto mesh = m_assetManager.getMeshes().getByName(name);

        m_assetsIdRedirections[oldId] = mesh->getId();

        LOG_INFO("Found mesh redirection: {} -> {}", oldId, mesh->getId());
    }

    deserializeDefaultAssets(assetsJson["default-assets"]);
}

void Deserializer::deserializeDefaultAssets(kc::json::Node& defaultAssets) {
    auto& globalShaders = glob::Globals::get().shaders->shadersByName;

    for (auto& shaderDescription : defaultAssets["shaders"]) {
        auto name = shaderDescription["name"].asString();

        if (globalShaders.contains(name)) {
            auto oldId = shaderDescription["id"].asString();
            auto newId = globalShaders.at(name)->getId();
            m_assetsIdRedirections[oldId] = newId;

            LOG_INFO("Found redirection for default shader: {} - {} -> {}", name, oldId, newId);
        }
    }

    auto& globalMeshes = glob::Globals::get().geom->meshes;

    for (auto& meshDescription : defaultAssets["meshes"]) {
        auto name = meshDescription["name"].asString();

        if (globalMeshes.contains(name)) {
            auto oldId = meshDescription["id"].asString();
            auto newId = globalMeshes.at(name)->getId();
            m_assetsIdRedirections[oldId] = newId;

            LOG_INFO("Found redirection for default mesh: {} - {} -> {}", name, oldId, newId);
        }
    }
}

void Deserializer::deserializeScene(kc::json::Node& sceneJson) {
    LOG_INFO("Deserializing scene");

    if (sceneJson.isMember("skybox-id")) {
        auto skyboxOldId = sceneJson["skybox-id"].asString();

        auto cubemap = m_assetManager.getCubemaps().getById(
            m_assetsIdRedirections.at(skyboxOldId));

        m_scene->skybox = scene::Skybox {
            glob::Globals::get().shaders->defaultCubemapShader, cubemap
        };

        LOG_INFO("Setting skybox: {}", m_scene->skybox->cubemap->getId());
    }

    LOG_INFO("Deserializing entities");
    for (auto& entityDescription : sceneJson["entities"]) {
        auto name = entityDescription["name"].asString();
        auto& entity = m_scene->addEntity(name);

        LOG_INFO("Deserializing entity: {}", name);
        LOG_INFO("Deserializing components");

        for (auto& componentDescription : entityDescription["components"]) {
            auto componentName = componentDescription["name"].asString();
            LOG_INFO("Deserializing {}", componentName);

            m_componentsDeserializer.deserializeComponent(componentName,
                componentDescription, entity, m_assetManager);
        }
    }
}
}
