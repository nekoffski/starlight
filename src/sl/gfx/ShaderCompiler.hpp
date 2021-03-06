#pragma once

#include <memory>

#include "ShaderCompilerImpl.h"

namespace sl::gfx {

class Shader;

struct ShaderCompiler {
    inline static std::unique_ptr<ShaderCompilerImpl> impl = nullptr;

    static void compile(Shader& shader) {
        impl->compile(shader);
    }
};
}
