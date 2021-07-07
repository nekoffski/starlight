#pragma once

#include "sl/core/WindowProxy.hpp"
#include "sl/gfx/LowLevelRenderer.h"
#include "sl/gfx/buffer/FrameBuffer.h"
#include "sl/scene/Scene.h"

namespace sl::rendering {

class Stage {
public:
    explicit Stage()
        : m_enabled(true) {
    }

    virtual void execute(gfx::LowLevelRenderer&, scene::Scene& scene, gfx::buffer::FrameBuffer*) = 0;

    void enable() {
        m_enabled = true;
    }

    void disable() {
        m_enabled = false;
    }

    bool isEnabled() const {
        return m_enabled;
    }

    void setWindowProxy(core::WindowProxy* windowProxy) {
        m_windowProxy = windowProxy;
    }

protected:
    core::WindowProxy* m_windowProxy;

private:
    bool m_enabled;
};

}