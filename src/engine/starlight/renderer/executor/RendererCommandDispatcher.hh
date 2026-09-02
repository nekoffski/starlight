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

   private:
    void dispatch(RendererNoop command);
    void dispatch(RendererFlush command);

    Renderer& m_renderer;
};

}  // namespace sl
