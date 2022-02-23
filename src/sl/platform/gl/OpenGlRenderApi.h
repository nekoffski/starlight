#pragma once

#include "sl/gfx/RenderApi.h"

namespace sl::platform::gl {

class OpenGlRenderApi : public sl::gfx::RenderApi {
   public:
    void init() override;
    void setViewport(const gfx::ViewFrustum::Viewport&) override;
    void clearBuffers(unsigned int buffers) override;

    void setCullFace(unsigned int) override;

    void drawArrays(unsigned, unsigned, unsigned) override;
    void drawElements(unsigned, unsigned, unsigned) override;

    void clearColor(float, float, float, float) override;

    void depthMask(bool) override;
    void setPolygonMode(unsigned) override;

    void setDepthFunc(unsigned) override;
    void setBlendFunc(unsigned, unsigned) override;

    void enable(unsigned) override;
    void disable(unsigned) override;

    glob::RendererInfo getRendererInfo() const override;

   private:
    glob::RendererInfo m_rendererInfo;
};

}  // namespace sl::platform::gl
