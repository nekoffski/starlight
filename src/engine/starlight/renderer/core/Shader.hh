#pragma once

#include <span>
#include <unordered_map>

#include "RenderResource.hh"
#include "ShaderDataLayout.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Enum.hh"
#include "starlight/core/Error.hh"

namespace sl {

enum class ShaderStage { vertex, fragment, compute, geometry };

struct ShaderModuleDescription {
    ShaderStage stage;
    Str entryPoint;
};

struct ShaderDescription {
    std::unordered_map<ShaderStage, ShaderModuleDescription> stages;
    std::vector<Byte> byteCode;
    Opt<VertexLayout> vertexLayout;
    std::vector<ShaderUniform> uniforms;
    std::vector<ShaderResourceTable> resourceTables;
};

struct ShaderParameterLayout {
    Str name;
    u64 size{0};
};

// An owned layout snapshot for one shader; it does not retain the GPU resource.
struct ShaderInterface {
    ShaderHandle handle;
    std::vector<ShaderParameterLayout> parameters;
};

// Validates a subset of blocks against an already validated layout.
Result<void> validateShaderParameterBlocks(
    std::span<const ShaderParameterLayout> layout,
    std::span<const ShaderParameterBlock> blocks
);

SL_BIT_ENUM(ShaderStage);

Str shaderStageToStr(ShaderStage stage);

Result<ShaderStage> shaderStageFromString(const Str& str);

}  // namespace sl
