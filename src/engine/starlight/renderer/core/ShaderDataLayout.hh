#pragma once

#include <cstring>
#include <type_traits>
#include <utility>
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

enum class ShaderResourceKind { texture, sampler };

struct ShaderResourceArray {
    ShaderResourceKind kind;
    u32 count;
    u32 offset;
    u32 stride;
    u32 binding;
};

struct ShaderResourceTable {
    Str name;
    u32 binding;
    u32 size;
    std::vector<ShaderResourceArray> arrays;
};

struct ShaderParameterBlock {
    Str name;
    Bytes bytes;

    template <typename T>
        requires std::is_trivially_copyable_v<T>
    static ShaderParameterBlock fromValue(Str name, const T& value) {
        Bytes bytes(sizeof(T));
        std::memcpy(bytes.data(), &value, sizeof(T));
        return {std::move(name), std::move(bytes)};
    }
};

struct ShaderUniform {
    u64 binding;
    u64 size;
    Str name;
};

}  // namespace sl
