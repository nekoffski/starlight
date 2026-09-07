#pragma once

#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"

namespace sl {

class RenderFrameRecorder : public NonCopyable, public NonMovable {
   public:
    virtual ~RenderFrameRecorder() = default;
};

}  // namespace sl
