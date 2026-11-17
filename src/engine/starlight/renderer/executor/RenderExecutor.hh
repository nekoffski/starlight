#pragma once

#include <kcore/Concepts.hh>
#include <kcore/runtime/Thread.hh>
#include <kcore/runtime/ThreadSafeQueue.hh>
#include <memory>

#include "RendererCommand.hh"
#include "RendererCommandDispatcher.hh"
#include "RendererCommandSubmitter.hh"
#include "starlight/core/Core.hh"

namespace sl {

class Renderer;

class RenderExecutor : public kc::NonCopyable, public kc::NonMovable {
    using Queue = kc::ThreadSafeQueue<RendererCommand>;

   public:
    explicit RenderExecutor(Renderer& renderer);
    ~RenderExecutor();

    void start();
    void stop();

    RendererCommandSubmitter createSubmitter();

   private:
    class DispatcherThread : public kc::Thread {
       public:
        explicit DispatcherThread(Renderer& renderer, Queue& queue);

       private:
        void run() override;

        Renderer& m_renderer;
        RendererCommandDispatcher m_dispatcher;
        Queue& m_queue;
    };

    std::shared_ptr<Queue> m_queue;
    DispatcherThread m_runner;
};

}  // namespace sl
