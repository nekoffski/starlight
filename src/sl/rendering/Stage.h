#pragma once

#include "sl/gfx/FrameBuffer.h"
#include "sl/gfx/Renderer.h"
#include "sl/scene/Scene.h"

namespace sl::rendering {

class Stage {
   public:
    explicit Stage() : m_enabled(true) {}

    virtual void execute(gfx::Renderer&, scene::Scene& scene, gfx::FrameBuffer*) = 0;

    void enable() { m_enabled = true; }

    void disable() { m_enabled = false; }

    bool isEnabled() const { return m_enabled; }

   private:
    bool m_enabled;
};

}  // namespace sl::rendering