#pragma once

#include "RendererCommand.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"
#include "starlight/renderer/backend/Renderer.hh"

namespace sl {

class RendererCommandDispatcher : public NonCopyable, public NonMovable {
   public:
    explicit RendererCommandDispatcher(Renderer& renderer);

    void dispatch(RendererCommand command);

    void operator()(RendererNoop command);
    void operator()(RendererFlush command);
    void operator()(RendererCreateSurfaceOutput command);

   private:
    Renderer& m_renderer;
};

}  // namespace sl
