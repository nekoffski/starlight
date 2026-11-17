#pragma once

#include <kcore/runtime/ThreadSafeQueue.hh>
#include <memory>

#include "RendererCommand.hh"

namespace sl {

class RendererCommandSubmitter {
    using Queue = kc::ThreadSafeQueue<RendererCommand>;

   public:
    explicit RendererCommandSubmitter(std::shared_ptr<Queue> queue);

    bool submit(RendererCommand command);

   private:
    std::weak_ptr<Queue> m_queue;
};

}  // namespace sl
