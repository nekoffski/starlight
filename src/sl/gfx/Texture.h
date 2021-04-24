#pragma once

#include <memory>

#include "sl/gfx/Image.h"

namespace sl::gfx {

class Texture {
public:
    struct Factory {
        virtual std::shared_ptr<Texture> create(const std::string&) = 0;
        virtual std::shared_ptr<Texture> create(unsigned int, unsigned int) = 0;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

    virtual ~Texture() = default;

    virtual void bind(unsigned int = 0) = 0;
    virtual void unbind() = 0;

    virtual unsigned int getWidth() const = 0;
    virtual unsigned int getHeight() const = 0;

    virtual unsigned int getBuffer() const = 0;

    virtual std::shared_ptr<gfx::Image> getImage() = 0;
};

}
