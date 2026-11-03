#pragma once

#include <kcore/Concepts.hh>

#include "starlight/core/Core.hh"

namespace sl {

class RenderSurfaceProvider : public kc::NonCopyable, public kc::NonMovable {
   public:
    virtual ~RenderSurfaceProvider() = default;
};

}  // namespace sl
