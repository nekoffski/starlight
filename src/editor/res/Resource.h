#pragma once

#include <memory>
#include <string>

#include "sl/geometry/Model.h"
#include "sl/graphics/Shader.h"
#include "sl/graphics/Cubemap.h"

namespace editor::res {

enum class ResourceType {
    CUBEMAP,
    TEXTURE,
    MODEL,
    SHADER
};

struct Resource : public std::enable_shared_from_this<Resource> {
    explicit Resource(std::string name)
        : name(std::move(name)) {
    }

    virtual ResourceType getType() = 0;

    template <typename T>
    std::shared_ptr<T> as() {
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }

    std::string name;
};

#define SL_RESOURCE(type) \
    ResourceType getType() override { return type; }

struct ShaderResource : public Resource {
    SL_RESOURCE(ResourceType::SHADER);

    explicit ShaderResource(std::shared_ptr<sl::graphics::Shader> shader, std::string name)
        : Resource(std::move(name))
        , shader(shader) {}

    std::shared_ptr<sl::graphics::Shader> shader;
};

struct CubemapResource : public Resource {
    SL_RESOURCE(ResourceType::CUBEMAP);

    explicit CubemapResource(std::shared_ptr<sl::graphics::Cubemap> cubemap, std::string name)
        : Resource(std::move(name))
        , cubemap(cubemap) {}

    std::shared_ptr<sl::graphics::Cubemap> cubemap;
};

struct ModelResource : public Resource {
    SL_RESOURCE(ResourceType::MODEL);

    explicit ModelResource(std::shared_ptr<sl::geometry::Model> model, std::string name)
        : Resource(std::move(name))
        , model(model) {}

    std::shared_ptr<sl::geometry::Model> model;
};
}
