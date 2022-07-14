#pragma once

namespace nova::gfx {

struct RendererBackend {
    virtual ~RendererBackend() {}

    virtual bool beginFrame(float deltaTime) = 0;
    virtual bool endFrame(float deltaTime)   = 0;

    virtual void onViewportResize(uint32_t width, uint32_t height) = 0;
};

}  // namespace nova::gfx
