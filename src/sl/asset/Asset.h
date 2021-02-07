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
    shader
};

struct Asset : public std::enable_shared_from_this<Asset> {
    explicit Asset(const std::string& name)
        : name(name) {
    }

    virtual AssetType getType() = 0;

    template <typename T>
    std::shared_ptr<T> as() {
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }

    std::string name;
};

#define SL_RESOURCE(type) \
    AssetType getType() override { return type; }

struct ShaderAsset : public Asset {
    SL_RESOURCE(AssetType::shader);

    explicit ShaderAsset(std::shared_ptr<sl::graphics::Shader> shader, const std::string& name)
        : Asset(name)
        , shader(shader) {}

    std::shared_ptr<sl::graphics::Shader> shader;
};

struct CubemapAsset : public Asset {
    SL_RESOURCE(AssetType::cubemap);

    explicit CubemapAsset(std::shared_ptr<sl::graphics::Cubemap> cubemap, const std::string& name)
        : Asset(name)
        , cubemap(cubemap) {}

    std::shared_ptr<sl::graphics::Cubemap> cubemap;
};

struct ModelAsset : public Asset {
    SL_RESOURCE(AssetType::model);

    explicit ModelAsset(std::shared_ptr<sl::geometry::Model> model, const std::string& name)
        : Asset(name)
        , model(model) {}

    std::shared_ptr<sl::geometry::Model> model;
};
}
