#pragma once

#include <variant>
#include <vector>

#include "starlight/core/Core.hh"
#include "starlight/core/FlatMap.hh"

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

struct StructType;

using DataType = std::variant<ValueType, StructType>;

struct StructType {
    FlatMap<Str, DataType> members;

    u64 size() const;
};

using VertexLayout = std::vector<ValueType>;

class LayoutHelper {
   public:
    static u64 calculateStride(const VertexLayout& layout);
    static u64 valueTypeSize(ValueType format);
    static u64 structTypeSize(const StructType& structType);
    static u64 dataTypeSize(const DataType& dataType);
};

Str vertexLayoutToString(const VertexLayout& layout);
Str valueTypeToString(ValueType format);

struct ShaderUniform {
    u64 binding;
    u64 size;
    Str name;
};

}  // namespace sl
