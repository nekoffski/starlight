#include "Shader.hh"

namespace sl {

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
