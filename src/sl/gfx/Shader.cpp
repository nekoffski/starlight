#include "Shader.h"

#include "ShaderManager.h"

namespace sl::gfx {

Shader::RecompileOnUpdate::RecompileOnUpdate(std::shared_ptr<gfx::Shader> shader, const kc::core::FileSystem& fileSystem)
    : m_shader(shader)
    , m_fileSystem(fileSystem)
    , m_vertexShaderPath(shader->getVertexShaderPath())
    , m_fragmentShaderPath(shader->getFragmentShaderPath())
    , m_previousFragmentWrite(m_fileSystem.getLastFileModificationTime(m_fragmentShaderPath))
    , m_previousVertexWrite(m_fileSystem.getLastFileModificationTime(m_vertexShaderPath))
    , m_wasCompiledCorrectly(true) {
}

bool Shader::RecompileOnUpdate::shouldInvoke() {
    if (auto shader = m_shader.lock(); shader) {
        auto newVertexWrite = m_fileSystem.getLastFileModificationTime(m_vertexShaderPath);
        auto newFragmentWrite = m_fileSystem.getLastFileModificationTime(m_fragmentShaderPath);

        auto shouldRecompile = m_previousVertexWrite != newVertexWrite ||
            m_previousFragmentWrite != newFragmentWrite;

        m_previousVertexWrite = newVertexWrite;
        m_previousFragmentWrite = newFragmentWrite;

        return shouldRecompile;
    }
    return false;
}

void Shader::RecompileOnUpdate::invoke() {
    if (auto shader = m_shader.lock(); shader) {
        try {
            ShaderManager::get().recompileShader(*shader);
            m_wasCompiledCorrectly = true;
        } catch (core::ShaderError& err) {
            if (m_wasCompiledCorrectly) {
                LOG_WARN("Could not recompile shader due to {}", err.getDetails());

                using namespace event;
                EventManager::get().emit<DisplayErrorEvent>(err.asString()).toAll();
            }
            m_wasCompiledCorrectly = false;
        }
    }
}

std::string Shader::RecompileOnUpdate::getName() const {
    return "RecompileShaderOnUpdate";
}

}