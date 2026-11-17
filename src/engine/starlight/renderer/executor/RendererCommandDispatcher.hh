#pragma once

#include <kcore/Concepts.hh>

#include "RendererCommand.hh"

namespace sl {

class Renderer;

class RendererCommandDispatcher : public kc::NonCopyable,
                                  public kc::NonMovable {
   public:
    explicit RendererCommandDispatcher(Renderer& renderer);

    void dispatch(RendererCommand command);

   private:
    Renderer& m_renderer;
};

}  // namespace sl
