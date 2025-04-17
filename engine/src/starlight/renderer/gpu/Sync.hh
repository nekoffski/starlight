#pragma once

#include "starlight/core/Core.hh"

#include "fwd.hh"

namespace sl {

struct Fence : public kstd::NonCopyable, public kstd::NonMovable {
    enum class State : u8 { signaled, notSignaled };

    static kstd::UniquePtr<Fence> create(State state);

    virtual ~Fence() = default;

    virtual bool wait(Nanoseconds timeout = max<u64>()) = 0;
    virtual void reset()                                = 0;
};

struct Semaphore : public kstd::NonCopyable, public kstd::NonMovable {
    static kstd::UniquePtr<Semaphore> create();

    virtual ~Semaphore() = default;
};

}  // namespace sl
