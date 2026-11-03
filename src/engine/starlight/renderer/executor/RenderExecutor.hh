#pragma once

#include <kcore/Concepts.hh>
#include <kcore/runtime/Thread.hh>
#include <kcore/runtime/ThreadSafeQueue.hh>
#include <memory>

#include "RendererCommand.hh"
#include "RendererCommandDispatcher.hh"
#include "starlight/core/Core.hh"
#include "starlight/renderer/frontend/Renderer.hh"

namespace sl {

class RenderExecutor : public kc::NonCopyable, public kc::NonMovable {
    using Queue = kc::ThreadSafeQueue<RendererCommand>;

   public:
    class DispatcherThread : public kc::Thread {
       public:
        explicit DispatcherThread(Renderer& renderer, Queue& queue);

       private:
        void run() override;

        Renderer& m_renderer;
        RendererCommandDispatcher m_dispatcher;
        Queue& m_queue;
    };

    class Submitter {
       public:
        explicit Submitter(std::shared_ptr<Queue> queue);

        bool submit(RendererCommand command);

       private:
        std::weak_ptr<Queue> m_queue;
    };

    explicit RenderExecutor(Renderer& renderer);
    ~RenderExecutor();

    void start();
    void stop();

    Submitter createSubmitter();

   private:
    std::shared_ptr<Queue> m_queue;
    DispatcherThread m_runner;
};

}  // namespace sl
