#pragma once

#include <kcore/Concepts.hh>
#include <kcore/io/FileSystem.hh>

#include "starlight/core/Error.hh"
#include "starlight/renderer/core/Shader.hh"

namespace sl {

class ShaderLoader : public kc::StaticClass {
   public:
    static Result<ShaderDescription> loadShaderDescription(
        const kc::Path& path
    );
};

}  // namespace sl
