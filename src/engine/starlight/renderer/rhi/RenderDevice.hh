#pragma once

#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"

namespace sl {

class RenderDevice : public NonCopyable, public NonMovable {
   public:
    virtual ~RenderDevice() = default;
};

}  // namespace sl
