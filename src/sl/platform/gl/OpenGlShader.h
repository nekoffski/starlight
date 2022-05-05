#pragma once

#include "sl/gpu/Shader.h"

namespace sl::platform::gl {

class OpenGlShaderCompiler;

class OpenGlShader : public sl::gpu::Shader {
    friend class OpenGlShaderCompiler;

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

    std::string getVertexShaderPath() override { return m_vertexPath; }

    std::string getFragmentShaderPath() override { return m_fragmentPath; }

    std::string getGeometryShaderPath() override { return m_geomPath; }

   private:
    unsigned int m_shaderProgram{0};

    std::string m_vertexPath;
    std::string m_fragmentPath;
    std::string m_geomPath;
};

struct OpenGlShaderFactory : sl::gpu::Shader::Factory {
    std::shared_ptr<sl::gpu::Shader> create(
        const std::string& vertex, const std::string& fragment, const std::string& geom
    ) {
        return std::make_shared<OpenGlShader>(vertex, fragment, geom);
    }
};
}  // namespace sl::platform::gl
