#include "RendererCommandDispatcher.hh"

#include <kcore/Functional.hh>

#include "starlight/renderer/frontend/Renderer.hh"

namespace sl {

RendererCommandDispatcher::RendererCommandDispatcher(Renderer& renderer)
    : m_renderer(renderer) {}

void RendererCommandDispatcher::dispatch(RendererCommand command) {
    const auto handlers = kc::Overloader{
        [](RendererNoop) {},

        [this](RendererFlush command) {
            m_renderer.flush();
            command.completion.set_value();
        },

        [this](RendererCreateSurfaceTarget command) {
            command.completion.set_value(
                m_renderer.createTarget(command.surfaceProvider)
            );
        },

        [this](RendererSubmit command) {
            command.completion.set_value(m_renderer.submit(command.request));
        },

        [this](RendererCreateShader command) {
            command.completion.set_value(
                m_renderer.createShader(command.description)
            );
        },

        [this](RendererDestroyShader command) {
            m_renderer.destroyShader(command.handle);
        },

        [this](RendererGetShaderInterface command) {
            command.completion.set_value(
                m_renderer.getShaderInterface(command.handle)
            );
        },

        [this](RendererCreatePrimitive command) {
            command.completion.set_value(
                m_renderer.createPrimitive(command.description)
            );
        },

        [this](RendererDestroyPrimitive command) {
            m_renderer.destroyPrimitive(command.primitive);
        },

        [this](RendererCreateTexture command) {
            command.completion.set_value(
                m_renderer.createTexture(command.description)
            );
        },

        [this](RendererDestroyTexture command) {
            m_renderer.destroyTexture(command.handle);
        },

        [this](RendererCreateSampler command) {
            command.completion.set_value(
                m_renderer.createSampler(command.description)
            );
        },

        [this](RendererDestroySampler command) {
            m_renderer.destroySampler(command.handle);
        }
    };
    std::visit(handlers, std::move(command));
}

}  // namespace sl
