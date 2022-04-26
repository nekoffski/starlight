#pragma once

#include <memory>

#include "sl/core/GameObject.h"
#include "sl/gfx/Image.h"

// todo: abstraction
#include "sl/platform/gl/OpenGlDefines.h"

namespace sl::gfx {

class Texture : public core::GameObject {
   public:
    inline static constexpr unsigned int shadowMapSize = 1024u * 4;

    struct Factory {
        virtual std::unique_ptr<Texture> create(sl::gfx::Image&) = 0;
        virtual std::unique_ptr<Texture> create(
            unsigned int, unsigned int, int = STARL_DEPTH_COMPONENT, int = STARL_DEPTH_COMPONENT
        ) = 0;
    };

    std::unique_ptr<Texture> clone();
    virtual ~Texture() = default;

    virtual void bind(unsigned int = 0) = 0;
    virtual void unbind()               = 0;

    std::pair<unsigned int, unsigned int> getSize() const;

    virtual unsigned int getWidth() const  = 0;
    virtual unsigned int getHeight() const = 0;

    virtual unsigned int getBuffer() const = 0;

    std::string name;
    std::string path;

    int internalFormat;
    int format;
};

}  // namespace sl::gfx
