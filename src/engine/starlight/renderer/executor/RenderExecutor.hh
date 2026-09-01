#pragma once

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

        RendererCommandDispatcher m_dispatcher;
        Queue& m_queue;
    };

    class Submitter : public NonCopyable, public NonMovable {
       public:
        constexpr Submitter(Queue& queue) : m_queue(queue) {}

        constexpr bool submit(RendererCommand command) {
            return m_queue.push(std::move(command));
        }

       private:
        Queue& m_queue;
    };

    explicit RenderExecutor(Renderer& renderer);

    void start();
    void stop();

    Submitter createSubmitter();

   private:
    Queue m_queue;
    DispatcherThread m_runner;
};

}  // namespace sl
