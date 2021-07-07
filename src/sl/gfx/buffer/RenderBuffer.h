#pragma once

#include <memory>

namespace sl::gfx::buffer {

struct RenderBuffer {
    struct Factory {
        virtual std::unique_ptr<RenderBuffer> create(int format, unsigned int width, unsigned int height) = 0;
    };

    virtual ~RenderBuffer() = default;

    virtual unsigned int getBufferId() const = 0;

    virtual void bind() = 0;
    virtual void unbind() = 0;

    inline static std::unique_ptr<Factory> factory = nullptr;
};

}