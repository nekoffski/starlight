#pragma once

namespace nova::gfx {

struct RendererBackend {
    virtual ~RendererBackend() {}

    virtual bool beginFrame() = 0;
    virtual bool endFrame()   = 0;
};

}  // namespace nova::gfx