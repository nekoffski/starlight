#pragma once

#include <memory>

#include "sl/async/Task.h"
#include "sl/core/FileSystem.h"
#include "sl/core/GameObject.h"
#include "sl/core/String.hpp"
#include "sl/event/Event.h"
#include "sl/event/EventManager.h"
#include "sl/math/Matrix.hpp"
#include "sl/math/Vector.hpp"

namespace sl::gfx {
class ShaderCompiler;

class Shader : public core::GameObject {
    friend class ShaderCompiler;

public:
    struct Factory {
        virtual std::shared_ptr<Shader> create(const std::string&, const std::string&, const std::string& = "") = 0;
        virtual ~Factory() = default;
    };

    // static std::shared_ptr<Shader> load(const std::string& vertex, const std::string& fragment, const std::string& geometry = "") {
    //     auto shader = factory->create(vertex, fragment, geometry);
    //     ShaderCompiler::compile(*shader);

    //     shader->name = core::extractNameFromPath(vertex);

    //     return shader;
    // }

    std::string name = "";

    // inline static std::unique_ptr<Factory> factory = nullptr;

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
        , m_fileSystem(std::move(fileSystem))
        , m_vertexShaderPath(shader->getVertexShaderPath())
        , m_fragmentShaderPath(shader->getFragmentShaderPath())
        , m_previousFragmentWrite(m_fileSystem->lastWriteTime(m_fragmentShaderPath))
        , m_previousVertexWrite(m_fileSystem->lastWriteTime(m_vertexShaderPath))
        , m_wasCompiledCorrectly(true) {
    }

    bool shouldInvoke() override {
        if (auto shader = m_shader.lock(); shader) {
            auto newVertexWrite = m_fileSystem->lastWriteTime(m_vertexShaderPath);
            auto newFragmentWrite = m_fileSystem->lastWriteTime(m_fragmentShaderPath);

            auto shouldRecompile = m_previousVertexWrite != newVertexWrite ||
                m_previousFragmentWrite != newFragmentWrite;

            m_previousVertexWrite = newVertexWrite;
            m_previousFragmentWrite = newFragmentWrite;

            return shouldRecompile;
        }
        return false;
    }

    void invoke() override {
        if (auto shader = m_shader.lock(); shader) {
            try {
                // gfx::ShaderCompiler::compile(*shader);
                m_wasCompiledCorrectly = true;
            } catch (core::ShaderError& err) {
                if (m_wasCompiledCorrectly) {
                    LOG_WARN("Could not recompile shader due to {}", err.getDetails());

                    using namespace event;
                    EventManager::get()->emit<DisplayErrorEvent>(err.as<std::string>()).toAll();
                }
                m_wasCompiledCorrectly = false;
            }
        }
    }

    std::string getName() const {
        return "RecompileShaderOnUpdate";
    }

private:
    std::weak_ptr<gfx::Shader> m_shader;
    std::unique_ptr<core::FileSystem> m_fileSystem;

    const std::string m_vertexShaderPath;
    const std::string m_fragmentShaderPath;

    core::FileSystem::TimeType m_previousVertexWrite;
    core::FileSystem::TimeType m_previousFragmentWrite;

    bool m_wasCompiledCorrectly;
};

}
