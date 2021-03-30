#pragma once

#include <memory>

#include "sl/core/Countable.hpp"
#include "sl/math/Matrix.hpp"
#include "sl/math/Vector.hpp"

namespace sl::gfx {
class ShaderCompilerImpl;

class Shader : public core::GameObject {
    friend class ShaderCompilerImpl;

public:
    struct Factory {
        virtual std::shared_ptr<Shader> create(const std::string&, const std::string&, const std::string& = "") = 0;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

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

    virtual std::string getVertexShaderPath() = 0;
    virtual std::string getFragmentShaderPath() = 0;
    virtual std::string getGeometryShaderPath() = 0;
};
}
