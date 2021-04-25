#include "Deserializer.h"

#include "sl/asset/Asset.h"
#include "sl/core/Errors.hpp"
#include "sl/core/Json.h"
#include "sl/core/Logger.h"
#include "sl/geom/ModelLoader.hpp"
#include "sl/gfx/Cubemap.h"
#include "sl/gfx/Shader.h"
#include "sl/scene/components/ComponentsDeserializers.hpp"
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

    for (auto& asset : assetsJson) {
        auto assetType = asset["type"].asInt();
        if (assetType > asset::AssetTypeMax) {
        }

        auto name = asset["name"].asString();
        auto id = asset["id"].asUInt();

        SL_INFO("Deserializing asset: {}/{}/{}", name, assetType, id);

        using asset::AssetType;

        switch (static_cast<AssetType>(assetType)) {
        case AssetType::shader: {
            auto& paths = asset["paths"];
            auto shader = gfx::Shader::load(
                paths[0].asString(), paths[1].asString(), paths[2].asString());
            shader->setId(id);
            auto shaderAsset = std::make_shared<asset::ShaderAsset>(shader, name);
            m_assetManager.addAsset(shaderAsset);

            break;
        }

        case AssetType::model: {
            auto path = asset["paths"][0].asString();
            auto model = geom::ModelLoader::load(path);
            model->setId(id);
            auto modelAsset = std::make_shared<asset::ModelAsset>(model, name);
            m_assetManager.addAsset(modelAsset);

            break;
        }

        case AssetType::cubemap: {
            auto& paths = asset["paths"];

            gfx::CubemapArgs faces;
            for (int i = 0; i < faces.size(); ++i)
                faces[i] = paths[i].asString();

            auto cubemap = gfx::Cubemap::load(faces);
            cubemap->setId(id);

            auto cubemapAsset = std::make_shared<asset::CubemapAsset>(cubemap, name);
            m_assetManager.addAsset(cubemapAsset);

            break;
        }
        }
    }
}

void Deserializer::deserializeScene(Json::Value& sceneJson) {
    SL_INFO("Deserializing scene");

    if (sceneJson.isMember("skybox-id")) {
        auto skyboxId = sceneJson["skybox-id"].asUInt();
        auto cubemapAsset = m_assetManager.getAssetById(skyboxId)->as<asset::CubemapAsset>();
        auto skybox = scene::Skybox::create(GLOBALS().shaders->defaultCubemapShader, cubemapAsset->cubemap);
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

            using scene::components::componentsDeserializers;

            if (not componentsDeserializers.contains(componentName)) {
                SL_WARN("Could not find deserializer for component: {}, skipping", componentName);
                continue;
            }

            SL_INFO("Loading component: {} for entity: {}", componentName, name);
            (componentsDeserializers[componentName])(entity, m_assetManager, componentDescription);
        }
    }
}
}
