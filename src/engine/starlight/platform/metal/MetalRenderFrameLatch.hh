#pragma once

#include <atomic>

#include "Metal.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"

namespace sl {

class MetalRenderFrameLatch : public NonCopyable, public NonMovable {
   public:
    bool tryAcquire();
    void release();

    void wait();

   private:
    std::atomic<bool> m_available{true};
};

}  // namespace sl
