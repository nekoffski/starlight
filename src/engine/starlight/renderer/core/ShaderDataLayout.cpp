#include "ShaderDataLayout.hh"

#include <unordered_map>

namespace sl {

Str valueTypeToString(const ValueType& type) {
    switch (type) {
        case ValueType::u8:
            return "u8";
        case ValueType::u16:
            return "u16";
        case ValueType::u32:
            return "u32";
        case ValueType::u64:
            return "u64";
        case ValueType::f32:
            return "f32";
        case ValueType::f32_3x1:
            return "f32_3x1";
        case ValueType::f32_4x1:
            return "f32_4x1";
        case ValueType::f32_4x4:
            return "f32_4x4";
    }
    std::unreachable();
}

Str vertexLayoutToString(const VertexLayout& layout) {
    Str str = "{";
    for (const auto& type : layout) {
        str += valueTypeToString(type) + ", ";
    }
    if (not layout.empty()) {
        str.pop_back();
        str.pop_back();
    }
    str += "}";
    return str;
}

}  // namespace sl
