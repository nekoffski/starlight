#pragma once

#include <memory>

#include "sl/async/Task.h"
#include "sl/core/FileSystem.h"
#include "sl/core/GameObject.h"
#include "sl/core/String.hpp"
#include "sl/math/Matrix.hpp"
#include "sl/math/Vector.hpp"

#include "ShaderCompiler.hpp"

namespace sl::gfx {
class ShaderCompilerImpl;

class Shader : public core::GameObject {
    friend class ShaderCompilerImpl;

public:
    struct Factory {
        virtual std::shared_ptr<Shader> create(const std::string&, const std::string&, const std::string& = "") = 0;
        virtual ~Factory() = default;
    };

    static std::shared_ptr<Shader> load(const std::string& vertex, const std::string& fragment, const std::string& geometry = "") {
        auto shader = factory->create(vertex, fragment, geometry);
        ShaderCompiler::compile(*shader);

        shader->name = core::extractNameFromPath(vertex);

        return shader;
    }

    std::string name = "";

    inline static std::unique_ptr<Factory> factory = nullptr;

    virtual ~Shader() = default;

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

class RecompileShaderOnUpdate : public async::Task {
public:
    explicit RecompileShaderOnUpdate(std::shared_ptr<gfx::Shader> shader,
        std::unique_ptr<core::FileSystem> fileSystem = std::make_unique<core::FileSystem>())
        : m_shader(shader)
        , m_fileSystem(std::move(fileSystem)) {
    }

    bool shouldInvoke() override {
        if (auto shader = m_shader.lock(); shader) {
            auto vertexShader = shader->getVertexShaderPath();
            auto fragmentShader = shader->getFragmentShaderPath();

            static auto previousVertexWrite = m_fileSystem->lastWriteTime(vertexShader);
            static auto previousFragmentWrite = m_fileSystem->lastWriteTime(fragmentShader);

            auto newVertexWrite = m_fileSystem->lastWriteTime(vertexShader);
            auto newFragmentWrite = m_fileSystem->lastWriteTime(fragmentShader);

            auto shouldRecompile = previousVertexWrite != newVertexWrite ||
                previousFragmentWrite != newFragmentWrite;

            previousVertexWrite = newVertexWrite;
            previousFragmentWrite = newFragmentWrite;

            return shouldRecompile;
        }
        return false;
    }

    void invoke() override {
        if (auto shader = m_shader.lock(); shader)
            gfx::ShaderCompiler::compile(*shader);
    }

    std::string getName() const {
        return "RecompileShaderOnUpdate";
    }

private:
    std::weak_ptr<gfx::Shader> m_shader;
    std::unique_ptr<core::FileSystem> m_fileSystem;
};

}
