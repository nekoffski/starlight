#pragma once

#include <memory>
#include <string>

#include "sl/geometry/Model.h"
#include "sl/graphics/Cubemap.h"
#include "sl/graphics/Shader.h"

namespace sl::asset {

enum class AssetType {
    cubemap,
    texture,
    model,
    shader,
    count
};

constexpr int AssetTypeMax = static_cast<int>(AssetType::count);

struct Asset : public std::enable_shared_from_this<Asset> {
    explicit Asset(const std::string& name)
        : name(name) {
    }

    virtual AssetType getType() = 0;
    virtual std::vector<std::string> getResourceLocation() = 0;
    virtual int getId() = 0;

    template <typename T>
    std::shared_ptr<T> as() {
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }

    std::string name;
    bool shouldSerialize = true;
};

#define SL_ASSET(type) \
    AssetType getType() override { return type; }

struct ShaderAsset : public Asset {
    SL_ASSET(AssetType::shader);

    explicit ShaderAsset(std::shared_ptr<sl::graphics::Shader> shader, const std::string& name)
        : Asset(name)
        , shader(shader) {}

    int getId() override {
        return shader->id;
    }

    std::vector<std::string> getResourceLocation() override {
        return {
            shader->getVertexShaderPath(),
            shader->getFragmentShaderPath(),
            shader->getGeometryShaderPath()
        };
    }

    std::shared_ptr<sl::graphics::Shader> shader;
};

struct CubemapAsset : public Asset {
    SL_ASSET(AssetType::cubemap);

    explicit CubemapAsset(std::shared_ptr<sl::graphics::Cubemap> cubemap, const std::string& name)
        : Asset(name)
        , cubemap(cubemap) {}

    int getId() override {
        return cubemap->id;
    }

    std::vector<std::string> getResourceLocation() override {
        auto faces = cubemap->getFaces();
        return std::vector<std::string>{ faces.begin(), faces.end() };
    }

    std::shared_ptr<sl::graphics::Cubemap> cubemap;
};

struct ModelAsset : public Asset {
    SL_ASSET(AssetType::model);

    explicit ModelAsset(std::shared_ptr<sl::geometry::Model> model, const std::string& name)
        : Asset(name)
        , model(model) {}

    int getId() override {
        return model->id;
    }

    std::vector<std::string> getResourceLocation() override {
        return { model->path };
    }

    std::shared_ptr<sl::geometry::Model> model;
};
}
