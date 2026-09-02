#include "Thread.hh"

#include <functional>

#include "starlight/core/Log.hh"
#include "starlight/core/Profiler.hh"
#include "starlight/core/Scope.hh"

namespace sl {

Thread::Thread(const Str& ident) : m_ident(ident) {}

Thread::~Thread() { join(); }

void Thread::join() {
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void Thread::sleepFor(std::chrono::milliseconds duration) {
    std::this_thread::sleep_for(duration);
}

void Thread::start() {
    m_thread = std::thread([&] { go(); });
}

void Thread::go() {
    SL_PROFILE_REGISTER_THREAD();
    log::info("{} - thread starting", m_ident);

    try {
        run();
    } catch (const std::exception& e) {
        log::error("{} - standard exception in thread: {}", m_ident, e.what());
        throw;
    } catch (...) {
        log::error("{} - unknown exception in thread", m_ident);
        throw;
    }
    log::info("{} - thread exiting", m_ident);
}

}  // namespace sl
