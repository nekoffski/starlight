#include "ShaderManager.h"

namespace sl::gfx {

ShaderManager::ShaderManager(ShaderCompiler* shaderCompiler, Shader::Factory* shaderFactory)
    : m_shaderCompiler(shaderCompiler), m_shaderFactory(shaderFactory) {}

std::shared_ptr<Shader> ShaderManager::load(const std::string& vertex, const std::string& fragment,
                                            const std::string& geometry) {
    auto shader = m_shaderFactory->create(vertex, fragment, geometry);
    m_shaderCompiler->compile(*shader);

    using namespace kc::core;

    shader->name = extractNameFromPath(vertex, ExtractingMode::withoutExtension);

    return shader;
}

void ShaderManager::recompileShader(Shader& shader) { m_shaderCompiler->compile(shader); }

}  // namespace sl::gfx