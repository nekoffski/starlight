#pragma once

#include <unordered_map>

#include "RenderResource.hh"
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
};

SL_BIT_ENUM(ShaderStage);

Str shaderStageToStr(ShaderStage stage);

Result<ShaderStage> shaderStageFromString(const Str& str);

}  // namespace sl
