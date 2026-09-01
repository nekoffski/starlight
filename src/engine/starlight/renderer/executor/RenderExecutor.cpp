#include "RenderExecutor.hh"

namespace sl {

RenderExecutor::RenderExecutor(Renderer& renderer)
    : m_runner(renderer, m_queue) {}

void RenderExecutor::start() { m_runner.start(); }

void RenderExecutor::stop() {
    m_queue.close();
    m_runner.join();
}

RenderExecutor::Submitter RenderExecutor::createSubmitter() {
    return Submitter{m_queue};
}

RenderExecutor::DispatcherThread::DispatcherThread(
    Renderer& renderer, Queue& queue
)
    : Thread("RenderExecutor"), m_dispatcher(renderer), m_queue(queue) {}

void RenderExecutor::DispatcherThread::run() {
    for (;;) {
        if (auto command = m_queue.pop(); not command) {
            log::info("renderer command queue has been closed");
            break;
        }
    }
}

}  // namespace sl
