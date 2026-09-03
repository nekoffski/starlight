#pragma once

#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"

namespace sl {

class RenderSurfaceProvider : public NonCopyable, public NonMovable {
   public:
    virtual ~RenderSurfaceProvider() = default;
};

}  // namespace sl
