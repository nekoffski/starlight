#include "Shader.hh"

#include <algorithm>
#include <string_view>
#include <unordered_set>

namespace sl {

Result<void> validateShaderParameterBlocks(
    std::span<const ShaderParameterLayout> layout,
    std::span<const ShaderParameterBlock> blocks
) {
    std::unordered_set<std::string_view> names;
    names.reserve(blocks.size());
    for (const auto& block : blocks) {
        const auto parameter =
            std::ranges::find(layout, block.name, &ShaderParameterLayout::name);
        if (parameter == layout.end()) {
            return Error::unexpected(
                ErrorCode::invalidArgument,
                "Unknown shader parameter block: {}", block.name
            );
        }
        if (block.bytes.size() != parameter->size) {
            return Error::unexpected(
                ErrorCode::invalidArgument,
                "Shader parameter block size mismatch for {}: expected {}, got "
                "{}",
                block.name, parameter->size, block.bytes.size()
            );
        }
        const bool uniqueName = names.insert(block.name).second;
        if (not uniqueName) {
            return Error::unexpected(
                ErrorCode::invalidArgument,
                "Duplicate shader parameter block: {}", block.name
            );
        }
    }
    return {};
}

Str shaderStageToStr(ShaderStage stage) {
    switch (stage) {
        case ShaderStage::vertex:
            return "vertex";
        case ShaderStage::fragment:
            return "fragment";
        case ShaderStage::compute:
            return "compute";
        case ShaderStage::geometry:
            return "geometry";
    }
    std::unreachable();
}

Result<ShaderStage> shaderStageFromString(const Str& str) {
    if (str == "vertex") {
        return ShaderStage::vertex;
    } else if (str == "fragment") {
        return ShaderStage::fragment;
    } else if (str == "compute") {
        return ShaderStage::compute;
    } else if (str == "geometry") {
        return ShaderStage::geometry;
    }
    return Error::unexpected(
        ErrorCode::invalidArgument, "Invalid shader module stage: {}", str
    );
}

}  // namespace sl
