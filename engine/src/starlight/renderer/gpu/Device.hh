#pragma once

#include "starlight/core/Core.hh"
#include "kstd/Singleton.hh"

#include "fwd.hh"
#include "Queue.hh"

namespace sl {

class Device : public kstd::Singleton<Device> {
public:
    struct Impl : kstd::NonCopyable, kstd::NonMovable {
        virtual ~Impl() = default;

        virtual void waitIdle()                   = 0;
        virtual Queue& getQueue(Queue::Type type) = 0;

        static kstd::UniquePtr<Impl> create();
    };

    explicit Device();

    void waitIdle();
    Queue& getQueue(Queue::Type type);

    Queue& getGraphicsQueue();
    Queue& getPresentQueue();

    Impl& getImpl();

private:
    kstd::UniquePtr<Impl> m_impl;
};

}  // namespace sl
