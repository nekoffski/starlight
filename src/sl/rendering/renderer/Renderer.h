#pragma once

#include "lowlevel/LowLevelRenderer.h"
#include "sl/rendering/camera/Camera.h"

namespace sl::rendering::renderer {

class Renderer {
public:
    explicit Renderer(lowlevel::LowLevelRenderer& lowLevelRenderer)
        : m_lowLevelRenderer(lowLevelRenderer) {
    }

    virtual ~Renderer() = default;

protected:
    lowlevel::LowLevelRenderer& m_lowLevelRenderer;
};
}
