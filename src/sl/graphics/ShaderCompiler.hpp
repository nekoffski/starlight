#pragma once

#include <memory>

#include "ShaderCompilerImpl.h"

namespace sl::graphics {

class Shader;

struct ShaderCompiler {
    inline static std::unique_ptr<ShaderCompilerImpl> impl = nullptr;

    static void compile(std::shared_ptr<Shader>& shader) {
		impl->compile(shader);
    }
};
}
