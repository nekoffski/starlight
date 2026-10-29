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

void RendererCommandDispatcher::operator()(RendererCreatePrimitive command) {
    command.completion.set_value(
        m_renderer.createPrimitive(command.description)
    );
}

void RendererCommandDispatcher::operator()(RendererDestroyPrimitive command) {
    m_renderer.destroyPrimitive(command.primitive);
}

void RendererCommandDispatcher::operator()(RendererCreateTexture command) {
    command.completion.set_value(m_renderer.createTexture(command.description));
}
void RendererCommandDispatcher::operator()(RendererDestroyTexture command) {
    m_renderer.destroyTexture(command.handle);
}

void RendererCommandDispatcher::operator()(RendererCreateSampler command) {
    command.completion.set_value(m_renderer.createSampler(command.description));
}
void RendererCommandDispatcher::operator()(RendererDestroySampler command) {
    m_renderer.destroySampler(command.handle);
}

}  // namespace sl
