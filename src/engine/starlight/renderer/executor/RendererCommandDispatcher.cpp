#include "RendererCommandDispatcher.hh"

namespace sl {

RendererCommandDispatcher::RendererCommandDispatcher(Renderer& renderer)
    : m_renderer(renderer) {}

void RendererCommandDispatcher::dispatch(RendererCommand command) {
    std::visit(
        [this](auto&& cmd) { dispatch(std::forward<decltype(cmd)>(cmd)); },
        std::move(command)
    );
}

void RendererCommandDispatcher::dispatch([[maybe_unused]] RendererNoop) {}

void RendererCommandDispatcher::dispatch(RendererFlush command) {
    m_renderer.flush();
    command.completion.set_value();
}

}  // namespace sl
