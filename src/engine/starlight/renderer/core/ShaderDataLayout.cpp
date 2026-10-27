#include "ShaderDataLayout.hh"

#include <unordered_map>

#include "starlight/core/Functional.hh"

namespace sl {

Str valueTypeToString(ValueType type) {
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

u64 LayoutHelper::valueTypeSize(ValueType format) {
    switch (format) {
        case ValueType::u8:
            return 1;
        case ValueType::u16:
            return 2;
        case ValueType::u32:
            return 4;
        case ValueType::u64:
            return 8;
        case ValueType::f32:
            return 4;
        case ValueType::f32_3x1:
            return 12;
        case ValueType::f32_4x1:
            return 16;
        case ValueType::f32_4x4:
            return 64;
    }
    std::unreachable();
}

u64 LayoutHelper::structTypeSize(const StructType& type) { return type.size(); }

u64 LayoutHelper::dataTypeSize(const DataType& type) {
    return std::visit(
        Overloader{
            [](const ValueType& value) {
                return LayoutHelper::valueTypeSize(value);
            },
            [](const StructType& value) {
                return LayoutHelper::structTypeSize(value);
            }
        },
        type
    );
}

u64 LayoutHelper::calculateStride(const VertexLayout& layout) {
    return std::accumulate(
        layout.begin(), layout.end(), u64{0u},
        [](u64 acc, ValueType format) -> u64 {
            return acc + LayoutHelper::valueTypeSize(format);
        }
    );
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

u64 StructType::size() const {
    u64 totalSize = 0;

    members.forEach([&](const DataType& dataType) {
        if (const auto* t = std::get_if<ValueType>(&dataType)) {
            totalSize += LayoutHelper::valueTypeSize(*t);
        } else if (const auto* t = std::get_if<StructType>(&dataType)) {
            totalSize += t->size();
        }
    });

    return totalSize;
}

}  // namespace sl
