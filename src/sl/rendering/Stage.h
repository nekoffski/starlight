#pragma once

#include "sl/gpu/FrameBuffer.h"
#include "sl/gpu/Renderer.h"
#include "sl/scene/Scene.h"

namespace sl::rendering {

class Stage {
   public:
    explicit Stage() : m_enabled(true) {}

    virtual void execute(gpu::Renderer&, scene::Scene& scene, gpu::FrameBuffer*) = 0;

    void enable() { m_enabled = true; }

    void disable() { m_enabled = false; }

    bool isEnabled() const { return m_enabled; }

   private:
    bool m_enabled;
};

}  // namespace sl::rendering