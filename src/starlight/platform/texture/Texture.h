#pragma once

#include <memory>

#include <starlight/platform/img/Image.h>

namespace starl::platform::texture {

class Texture {
public:
    static std::shared_ptr<Texture> create(const std::string&);

    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual std::shared_ptr<img::Image> getImage() = 0;
};
}