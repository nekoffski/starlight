#include "Shader.h"

namespace sl {
void Shader::setGlobalUniforms(SelfCallback&& callback) {
    bindGlobals();
    callback(this);
    applyGlobals();
}

void Shader::setInstanceUniforms(u32 instanceId, SelfCallback&& callback) {
    bindInstance(instanceId);
    callback(this);
    applyInstance();
}

void Shader::setLocalUniforms(SelfCallback&& callback) {
    // nothing to do here, just for consistency
    callback(this);
}

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

void Shader::setUniform(const std::string& uniform, Texture* value) {
    setUniform(uniform, static_cast<void*>(value));
}

}  // namespace sl
