#pragma once

#include <memory>

#include "sl/core/GameObject.h"
#include "sl/gfx/Image.h"

namespace sl::gfx {

class Texture : public core::GameObject {
public:
    struct Factory {
        virtual std::shared_ptr<Texture> create(const std::string&) = 0;
        virtual std::shared_ptr<Texture> create(unsigned int, unsigned int) = 0;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

    static std::shared_ptr<Texture> load(const std::string& path) {
        auto texture = factory->create(path);
        texture->path = path;

        return texture;
    }

    virtual ~Texture() = default;

    virtual void bind(unsigned int = 0) = 0;
    virtual void unbind() = 0;

    virtual unsigned int getWidth() const = 0;
    virtual unsigned int getHeight() const = 0;

    virtual unsigned int getBuffer() const = 0;

    virtual std::shared_ptr<gfx::Image> getImage() = 0;

    std::string name;
    std::string path;
};

}
