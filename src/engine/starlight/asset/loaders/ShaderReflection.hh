#pragma once

#include <kcore/serialization/Json.hh>

#include "starlight/renderer/core/Shader.hh"

namespace sl {

struct ShaderReflection {
    static Result<ShaderReflection> parse(const kc::Json& metadata);

    std::unordered_map<ShaderStage, ShaderModuleDescription> stages;
    Opt<VertexLayout> vertexLayout;
    std::vector<ShaderUniform> uniforms;
    std::vector<ShaderResourceTable> resourceTables;
};

}  // namespace sl
