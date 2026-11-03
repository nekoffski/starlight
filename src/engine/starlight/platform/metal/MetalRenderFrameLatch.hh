#pragma once

#include <atomic>
#include <kcore/Concepts.hh>

#include "Metal.hh"
#include "starlight/core/Core.hh"

namespace sl {

class MetalRenderFrameLatch : public kc::NonCopyable, public kc::NonMovable {
   public:
    bool tryAcquire();
    void release();

    void wait();

   private:
    std::atomic<bool> m_available{true};
};

}  // namespace sl
