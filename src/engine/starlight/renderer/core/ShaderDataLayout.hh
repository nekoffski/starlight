#pragma once

#include <variant>
#include <vector>

#include "starlight/core/Core.hh"

namespace sl {

enum class DataFormat {
    u8,
    u16,
    u32,
    u64,
    f32,
    f32_3x1,
    f32_4x1,
    f32_4x4,
};

using VertexLayout = std::vector<DataFormat>;

class LayoutHelper {
   public:
    static u64 calculateStride(const VertexLayout& layout);
    static u64 formatSize(DataFormat format);
};

Str vertexLayoutToString(const VertexLayout& layout);
Str dataFormatToString(DataFormat format);

}  // namespace sl
