#pragma once

#include "sl/graphics/Shader.h"

namespace sl::platform::shader {

class OpenGlShaderCompilerImpl;

class OpenGlShader : public graphics::Shader {
    friend class OpenGlShaderCompilerImpl;

public:
    explicit OpenGlShader(const std::string&, const std::string&, const std::string&);

    ~OpenGlShader() override;

    void enable() override;
    void disable() override;

    void setUniform(const std::string&, float) override;
    void setUniform(const std::string&, int) override;
    void setUniform(const std::string&, unsigned int) override;
    void setUniform(const std::string&, math::Mat4) override;
    void setUniform(const std::string&, math::Mat3) override;
    void setUniform(const std::string&, math::Vec4) override;
    void setUniform(const std::string&, math::Vec3) override;

    std::string getVertexShaderPath() override {
        return m_vertexPath;
    }

    std::string getFragmentShaderPath() override {
        return m_fragmentPath;
    }

    std::string getGeometryShaderPath() override {
        return m_geometryPath;
    }

private:
    unsigned int m_shaderProgram{ 0 };

    std::string m_vertexPath;
    std::string m_fragmentPath;
    std::string m_geometryPath;
};

struct OpenGlShaderFactory : graphics::Shader::Factory {
    std::shared_ptr<graphics::Shader> create(const std::string& vertex, const std::string& fragment, const std::string& geometry) {
        return std::make_shared<OpenGlShader>(vertex, fragment, geometry);
    }
};
}
