#pragma once

#include <memory>

#include "LowLevelRenderer.h"

namespace sl::gfx {

class LowLevelRendererProxy {
public:
    explicit LowLevelRendererProxy(LowLevelRenderer* lowLevelRenderer)
        : m_lowLevelRenderer(lowLevelRenderer) {
    }

    void setViewport(const ViewFrustum::Viewport& viewport) {
        m_lowLevelRenderer->setViewport(viewport);
    }

private:
    LowLevelRenderer* m_lowLevelRenderer;
};

}