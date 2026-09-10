#pragma once

#include "starlight/core/Concepts.hh"
#include "starlight/core/Error.hh"
#include "starlight/core/FileSystem.hh"
#include "starlight/renderer/rhi/Shader.hh"

namespace sl {

class ShaderLoader : public StaticClass {
   public:
    static Result<ShaderDescription> loadShaderDescription(const Path& path);
};

}  // namespace sl
