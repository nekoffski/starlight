#include "RenderExecutor.hh"

#include "starlight/renderer/frontend/Renderer.hh"

namespace sl {

RenderExecutor::RenderExecutor(Renderer& renderer)
    : m_queue(std::make_shared<Queue>()), m_runner(renderer, *m_queue) {}

RenderExecutor::~RenderExecutor() { stop(); }

void RenderExecutor::start() { m_runner.start(); }

void RenderExecutor::stop() {
    m_queue->close();
    m_runner.join();
}

RendererCommandSubmitter RenderExecutor::createSubmitter() {
    return RendererCommandSubmitter{m_queue};
}

RenderExecutor::DispatcherThread::DispatcherThread(
    Renderer& renderer, Queue& queue
)
    : kc::Thread("RenderExecutor"),
      m_renderer(renderer),
      m_dispatcher(renderer),
      m_queue(queue) {}

void RenderExecutor::DispatcherThread::run() {
    for (;;) {
        if (m_queue.closed()) {
            log::info("renderer command queue has been closed");
            break;
        }

        if (auto command = m_queue.tryPop(); command) [[likely]] {
            m_dispatcher.dispatch(std::move(*command));
        }
        m_renderer.tick();
    }
    m_renderer.flush();
}

}  // namespace sl
