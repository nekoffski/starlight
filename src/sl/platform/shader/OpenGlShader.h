#pragma once

#include "sl/graphics/Shader.h"

namespace sl::platform::shader {

class OpenGlShaderCompilerImpl;

class OpenGlShader : public graphics::Shader {
    friend class OpenGlShaderCompilerImpl;

public:
    explicit OpenGlShader(std::string, std::string, std::string);

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

private:
    unsigned int m_shaderProgram{ 0 };

    std::string m_vertexPath;
    std::string m_fragmentPath;
    std::string m_geometryPath;
};

struct OpenGlShaderFactory : graphics::Shader::Factory {
	std::shared_ptr<graphics::Shader> create(std::string vertex, std::string fragment, std::string geometry) {
		return std::make_shared<OpenGlShader>(vertex, fragment, geometry);
	}
};

}
