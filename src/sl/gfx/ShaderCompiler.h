#pragma once

#include "fwd.h"

namespace sl::gfx {

class ShaderCompiler {
   public:
    virtual ~ShaderCompiler() = default;

    virtual void compile(Shader&) = 0;
};
}  // namespace sl::gfx
