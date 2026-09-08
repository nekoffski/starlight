#pragma once

#include <span>

#include "RenderResource.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Error.hh"
#include "starlight/core/Functional.hh"
#include "starlight/math/Math.hh"

namespace sl {

enum class LoadOp { load, clear, discard };
enum class StoreOp { store, discard };

struct ColorAttachment {
    RenderTarget target;
    LoadOp loadOp{LoadOp::load};
    StoreOp storeOp{StoreOp::store};
    Vec4f clearColor;
};

struct RenderPassDescription {
    Str label;
    std::span<const ColorAttachment> colorAttachments;
};

class RenderPassEncoder : public NonCopyable, public NonMovable {
   public:
    virtual ~RenderPassEncoder() = default;
};

using RenderPassCallback = MoveOnlyFunction<Result<void>(RenderPassEncoder&)>;

}  // namespace sl
