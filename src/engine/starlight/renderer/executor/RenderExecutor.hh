#pragma once

#include <memory>

#include "RendererCommand.hh"
#include "RendererCommandDispatcher.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"
#include "starlight/renderer/backend/Renderer.hh"
#include "starlight/runtime/Thread.hh"
#include "starlight/runtime/ThreadSafeQueue.hh"

namespace sl {

class RenderExecutor : public NonCopyable, public NonMovable {
    using Queue = ThreadSafeQueue<RendererCommand>;

   public:
    class DispatcherThread : public Thread {
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
