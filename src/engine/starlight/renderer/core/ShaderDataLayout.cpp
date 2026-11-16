#include "ShaderDataLayout.hh"

#include <algorithm>
#include <array>
#include <kcore/Functional.hh>

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
        case ValueType::f32_2x1:
            return "f32_2x1";
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
        case ValueType::f32_2x1:
            return 8;
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
        kc::Overloader{
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

namespace {

bool isVertexType(ValueType type) {
    return type == ValueType::f32_2x1 || type == ValueType::f32_3x1 ||
           type == ValueType::f32_4x1;
}

}  // namespace

Result<void> validateVertexLayout(const VertexLayout& layout) {
    constexpr u32 componentAlignment = sizeof(f32);
    const bool validStride = layout.stride > 0 &&
                             layout.stride <= maxVertexStride &&
                             layout.stride % componentAlignment == 0;
    if (not validStride || layout.attributes.empty() ||
        layout.attributes.size() > maxVertexAttributes) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument,
            "Invalid vertex stride or attribute count"
        );
    }
    std::array<bool, maxVertexAttributes> seen{};
    for (const auto& attribute : layout.attributes) {
        if (attribute.location >= maxVertexAttributes ||
            not isVertexType(attribute.type)) {
            return Error::unexpected(
                kc::ErrorCode::invalidArgument,
                "Unsupported vertex location or type"
            );
        }
        if (std::exchange(seen[attribute.location], true)) {
            return Error::unexpected(
                kc::ErrorCode::invalidArgument,
                "Duplicate vertex attribute location"
            );
        }
        const auto size = LayoutHelper::valueTypeSize(attribute.type);
        const bool fitsStride = attribute.offset <= layout.stride &&
                                size <= layout.stride - attribute.offset;
        if (not fitsStride || attribute.offset % componentAlignment != 0) {
            return Error::unexpected(
                kc::ErrorCode::invalidArgument,
                "Vertex attribute exceeds stride or is misaligned"
            );
        }
    }
    return {};
}

Result<void> validateVertexInputs(std::span<const VertexInput> inputs) {
    std::array<bool, maxVertexAttributes> seen{};
    for (const auto& input : inputs) {
        if (input.location >= maxVertexAttributes ||
            not isVertexType(input.type)) {
            return Error::unexpected(
                kc::ErrorCode::invalidArgument,
                "Unsupported shader vertex input"
            );
        }
        if (std::exchange(seen[input.location], true)) {
            return Error::unexpected(
                kc::ErrorCode::invalidArgument,
                "Duplicate shader vertex input location"
            );
        }
    }
    return {};
}

Result<void> validateVertexCompatibility(
    const VertexLayout& layout, std::span<const VertexInput> inputs
) {
    auto r = validateVertexLayout(layout);
    if (not r) {
        return r;
    }
    r = validateVertexInputs(inputs);
    if (not r) {
        return r;
    }
    for (const auto& input : inputs) {
        const auto attribute = std::ranges::find(
            layout.attributes, input.location, &VertexAttribute::location
        );
        if (attribute == layout.attributes.end() ||
            attribute->type != input.type) {
            return Error::unexpected(
                kc::ErrorCode::invalidArgument,
                "Geometry does not supply a required shader vertex input"
            );
        }
    }
    return {};
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
