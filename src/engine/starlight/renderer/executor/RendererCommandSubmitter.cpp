#include "RendererCommandSubmitter.hh"

namespace sl {

RendererCommandSubmitter::RendererCommandSubmitter(std::shared_ptr<Queue> queue)
    : m_queue(std::move(queue)) {}

bool RendererCommandSubmitter::submit(RendererCommand command) {
    if (auto queue = m_queue.lock(); queue) {
        return queue->push(std::move(command));
    }
    return false;
}

}  // namespace sl
