#pragma once

#include <string>
#include <memory>

#include "sl/platform/texture/Cubemap.h"

namespace editor::res {

enum class ResourceType {
    CUBEMAP,
    TEXTURE,
    MODEL
};

struct Resource : public std::enable_shared_from_this<Resource> {
    explicit Resource(std::string name)
        : name(std::move(name)) {
    }

    virtual ResourceType getType() = 0;

	template<typename T>
	std::shared_ptr<T> as () {
		return std::dynamic_pointer_cast<T>(shared_from_this());
	}


    std::string name;
};

#define SL_RESOURCE(type) \
    ResourceType getType() override { return type; }

struct CubemapResource : public Resource {
    SL_RESOURCE(ResourceType::CUBEMAP);

    explicit CubemapResource(std::shared_ptr<sl::platform::texture::Cubemap> cubemap, std::string name)
        : Resource(std::move(name))
        , cubemap(cubemap) {}

    std::shared_ptr<sl::platform::texture::Cubemap> cubemap;
};
}
