#pragma once

#include <memory>
#include <string>

namespace starl::platform::texture {

using CubemapArgs = std::array<std::string, 6>;

class Cubemap {
public:
    static std::shared_ptr<Cubemap> create(const CubemapArgs);

    virtual ~Cubemap() = default;

    virtual void bind() = 0;
    virtual void unbind() = 0;
};
}