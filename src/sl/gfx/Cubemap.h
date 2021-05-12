#pragma once

#include <memory>
#include <string>
#include <vector>

#include "sl/core/GameObject.h"

namespace sl::gfx {

using CubemapArgs = std::array<std::string, 6>;

class Cubemap : public core::GameObject {
public:
    struct Factory {
        virtual std::shared_ptr<Cubemap> create(const CubemapArgs&) = 0;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

    static std::shared_ptr<Cubemap> load(const CubemapArgs& args) {
        return factory->create(args);
    }

    virtual ~Cubemap() = default;

    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual CubemapArgs getFaces() = 0;

    std::string name;
};
}
