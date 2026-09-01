#pragma once

#include <atomic>
#include <thread>
#include <vector>

#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Error.hh"

namespace sl {

class Thread : public NonCopyable, public NonMovable {
   public:
    enum class Status { created, running, failed, finished };

    explicit Thread(const Str& ident);
    virtual ~Thread();

    void start();
    Result<void> join();
    Status status() const;
    virtual void cancel() {}

   protected:
    void sleepFor(std::chrono::milliseconds duration);

   private:
    virtual Result<void> run() = 0;
    void go();

    Str m_ident;
    std::atomic<Status> m_status{Status::created};
    Opt<Error> m_error;
    std::thread m_thread;
};

}  // namespace sl
