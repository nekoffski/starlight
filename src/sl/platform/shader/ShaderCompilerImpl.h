#pragma once

#include <memory>

namespace sl::platform::shader {

class Shader;

class ShaderCompilerImpl {
public:
    static std::unique_ptr<ShaderCompilerImpl> create();

    virtual void compile(std::shared_ptr<Shader>&) = 0;
};
}