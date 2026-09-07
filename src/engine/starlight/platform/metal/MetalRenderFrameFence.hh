#pragma once

#include <atomic>

#include "Metal.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"

namespace sl {

class MetalRenderFrameFence : public NonCopyable, public NonMovable {
   public:
    bool tryAcquire();
    void release();

   private:
    std::atomic<bool> m_available{true};
};

}  // namespace sl
