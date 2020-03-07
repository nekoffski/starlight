#pragma once

#include <memory>

namespace starl::platform::shader {

class Shader;

class ShaderCompilerImpl {
public:
    static std::unique_ptr<ShaderCompilerImpl> create();

    virtual void compile(std::shared_ptr<Shader>&) = 0;

protected:
    std::string getVertexShaderPath(std::shared_ptr<Shader>&);
    std::string getFragmentShaderPath(std::shared_ptr<Shader>&);
    std::string getGeometryShaderPath(std::shared_ptr<Shader>&);
};
}