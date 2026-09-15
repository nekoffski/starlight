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
    RendererCreateSurfaceTarget command
) {
    command.completion.set_value(
        m_renderer.createTarget(command.surfaceProvider)
    );
}

void RendererCommandDispatcher::operator()(RendererSubmit command) {
    command.completion.set_value(m_renderer.submit(command.request));
}

void RendererCommandDispatcher::operator()(RendererCreateShader command) {
    command.completion.set_value(m_renderer.createShader(command.description));
}

void RendererCommandDispatcher::operator()(RendererDestroyShader command) {
    m_renderer.destroyShader(command.handle);
}

}  // namespace sl
