#pragma once

#include "starlight/core/Core.hh"
#include "starlight/renderer/core/RenderResource.hh"

namespace sl {

struct IndexRange {
    u32 firstIndex{0};
    u32 indexCount{0};

    bool operator==(const IndexRange&) const = default;
};

// Non-owning draw selection; no range means the whole uploaded primitive.
struct PrimitiveDraw {
    PrimitiveHandle primitive;
    Opt<IndexRange> indexRange;

    bool operator==(const PrimitiveDraw&) const = default;
};

}  // namespace sl
