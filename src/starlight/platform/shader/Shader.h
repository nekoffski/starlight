#pragma once

#include <memory>

#include <starlight/math/Matrix.hpp>
#include <starlight/math/Vector.hpp>

namespace starl::platform::shader {
class ShaderCompilerImpl;

class Shader {
    friend class ShaderCompilerImpl;

public:
    static std::shared_ptr<Shader> create(std::string, std::string, std::string = "");

    virtual ~Shader() {}

    virtual void enable() = 0;
    virtual void disable() = 0;

    virtual void setUniform(const std::string&, float) = 0;
    virtual void setUniform(const std::string&, int) = 0;
    virtual void setUniform(const std::string&, unsigned int) = 0;
    virtual void setUniform(const std::string&, math::Mat4) = 0;
    virtual void setUniform(const std::string&, math::Mat3) = 0;
    virtual void setUniform(const std::string&, math::Vec4) = 0;
    virtual void setUniform(const std::string&, math::Vec3) = 0;
};
}
