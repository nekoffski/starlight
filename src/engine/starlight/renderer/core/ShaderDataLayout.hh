#pragma once

#include <cstring>
#include <kcore/containers/FlatMap.hh>
#include <span>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "starlight/core/Error.hh"

namespace sl {

enum class ValueType {
    u8,
    u16,
    u32,
    u64,
    f32,
    f32_2x1,
    f32_3x1,
    f32_4x1,
    f32_4x4,
};

struct StructType;

using DataType = std::variant<ValueType, StructType>;

struct StructType {
    kc::FlatMap<Str, DataType> members;

    u64 size() const;
};

// Conservative limits shared by the Metal and target Vulkan vertex paths.
constexpr u32 maxVertexAttributes = 16;
constexpr u32 maxVertexStride = 2048;

struct VertexInput {
    u32 location;
    ValueType type;

    bool operator==(const VertexInput&) const = default;
};

using ShaderVertexInputs = std::vector<VertexInput>;

struct VertexAttribute {
    u32 location;
    ValueType type;
    u32 offset;

    bool operator==(const VertexAttribute&) const = default;
};

struct VertexLayout {
    u32 stride{0};
    std::vector<VertexAttribute> attributes;

    bool operator==(const VertexLayout&) const = default;
};

Result<void> validateVertexLayout(const VertexLayout& layout);
Result<void> validateVertexInputs(std::span<const VertexInput> inputs);
Result<void> validateVertexCompatibility(
    const VertexLayout& layout, std::span<const VertexInput> inputs
);

class LayoutHelper {
   public:
    static u64 valueTypeSize(ValueType format);
    static u64 structTypeSize(const StructType& structType);
    static u64 dataTypeSize(const DataType& dataType);
};

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
