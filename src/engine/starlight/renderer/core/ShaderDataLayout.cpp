#include "ShaderDataLayout.hh"

#include <unordered_map>

namespace sl {

Str dataFormatToString(DataFormat type) {
    switch (type) {
        case DataFormat::u8:
            return "u8";
        case DataFormat::u16:
            return "u16";
        case DataFormat::u32:
            return "u32";
        case DataFormat::u64:
            return "u64";
        case DataFormat::f32:
            return "f32";
        case DataFormat::f32_3x1:
            return "f32_3x1";
        case DataFormat::f32_4x1:
            return "f32_4x1";
        case DataFormat::f32_4x4:
            return "f32_4x4";
    }
    std::unreachable();
}

u64 LayoutHelper::formatSize(DataFormat format) {
    switch (format) {
        case DataFormat::u8:
            return 1;
        case DataFormat::u16:
            return 2;
        case DataFormat::u32:
            return 4;
        case DataFormat::u64:
            return 8;
        case DataFormat::f32:
            return 4;
        case DataFormat::f32_3x1:
            return 12;
        case DataFormat::f32_4x1:
            return 16;
        case DataFormat::f32_4x4:
            return 64;
    }
    std::unreachable();
}

u64 LayoutHelper::calculateStride(const VertexLayout& layout) {
    return std::accumulate(
        layout.begin(), layout.end(), u64{0u},
        [](u64 acc, DataFormat format) -> u64 {
            return acc + LayoutHelper::formatSize(format);
        }
    );
}

Str vertexLayoutToString(const VertexLayout& layout) {
    Str str = "{";
    for (const auto& type : layout) {
        str += dataFormatToString(type) + ", ";
    }
    if (not layout.empty()) {
        str.pop_back();
        str.pop_back();
    }
    str += "}";
    return str;
}

}  // namespace sl
