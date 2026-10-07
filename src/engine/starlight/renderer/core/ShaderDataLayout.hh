#pragma once

#include <variant>
#include <vector>

#include "starlight/core/Core.hh"

namespace sl {

enum class ValueType {
    u8,
    u16,
    u32,
    u64,
    f32,
    f32_3x1,
    f32_4x1,
    f32_4x4,
};

Str valueTypeToString(const ValueType& type);

using VertexLayout = std::vector<ValueType>;

Str vertexLayoutToString(const VertexLayout& layout);

}  // namespace sl
