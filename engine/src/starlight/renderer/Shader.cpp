#include "Shader.h"

namespace sl {

void Shader::use() { impl->use(); }

void Shader::bindGlobals() { impl->bindGlobals(); }

void Shader::bindInstance(u32 instanceId) { impl->bindInstance(instanceId); }

void Shader::applyGlobals() { impl->applyGlobals(); }

void Shader::applyInstance() { impl->applyInstance(); }

u32 Shader::acquireInstanceResources() { return impl->acquireInstanceResources(); }

void Shader::releaseInstanceResources(u32 instanceId) {
    impl->releaseInstanceResources(instanceId);
}

void Shader::setUniform(const std::string& uniform, void* value) {
    impl->setUniform(uniforms[uniform], value);
}

}  // namespace sl
