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
    explicit Thread(const Str& ident);
    virtual ~Thread();

    void start();
    void join();

   protected:
    void sleepFor(std::chrono::milliseconds duration);

   private:
    virtual void run() = 0;
    void go();

    Str m_ident;
    std::thread m_thread;
};

}  // namespace sl
