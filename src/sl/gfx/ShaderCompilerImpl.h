#pragma once

#include <memory>

namespace sl::gfx {

class Shader;

class ShaderCompilerImpl {
public:
    struct Factory {
        virtual std::unique_ptr<ShaderCompilerImpl> create() = 0;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

    virtual void compile(std::shared_ptr<Shader>&) = 0;
};
}
