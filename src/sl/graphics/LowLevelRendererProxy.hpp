#pragma once

#include <memory>

#include "LowLevelRenderer.h"

namespace sl::graphics {

class LowLevelRendererProxy {
public:
    explicit LowLevelRendererProxy(std::shared_ptr<LowLevelRenderer> lowLevelRenderer)
        : m_lowLevelRenderer(lowLevelRenderer) {
    }

    void setViewport(const ViewFrustum::Viewport& viewport) {
        m_lowLevelRenderer->setViewport(viewport);
    }

private:
    std::shared_ptr<LowLevelRenderer> m_lowLevelRenderer;
};

}