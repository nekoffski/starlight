#pragma once

#include <memory>

namespace starl::platform::shader {
class ShaderCompilerImpl;

class Shader {
    friend class ShaderCompilerImpl;

public:
    static std::shared_ptr<Shader> create(std::string, std::string, std::string = "");

    explicit Shader(std::string, std::string, std::string);

    virtual void enable() = 0;
    virtual void disable() = 0;

protected:
    std::string m_vertexPath;
    std::string m_fragmentPath;
    std::string m_geometryPath;
};
}
