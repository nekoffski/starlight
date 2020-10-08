#pragma once

#include <memory>

#include "sl/platform/img/Image.h"

namespace sl::platform::texture {

class Texture {
public:
    static std::shared_ptr<Texture> create(const std::string&);
    static std::shared_ptr<Texture> create(unsigned int w, unsigned int h);

    virtual ~Texture() = default;

    virtual void bind(unsigned int = 0) = 0;
    virtual void unbind() = 0;

    virtual unsigned int getWidth() const = 0;
    virtual unsigned int getHeight() const = 0;

    virtual unsigned int getBuffer() const = 0;

    virtual std::shared_ptr<img::Image> getImage() = 0;
};
}