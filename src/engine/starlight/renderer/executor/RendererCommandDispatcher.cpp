#include "RendererCommandDispatcher.hh"

namespace sl {

RendererCommandDispatcher::RendererCommandDispatcher(Renderer& renderer)
    : m_renderer(renderer) {}

void RendererCommandDispatcher::dispatch(RendererCommand command) {
    std::visit(*this, std::move(command));
}

void RendererCommandDispatcher::operator()(RendererNoop) {}

void RendererCommandDispatcher::operator()(RendererFlush command) {
    m_renderer.flush();
    command.completion.set_value();
}

void RendererCommandDispatcher::operator()(
    RendererCreateSurfaceOutput command
) {
    command.completion.set_value(m_renderer.createOutput(command.surfaceProvider));
}

}  // namespace sl
