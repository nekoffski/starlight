#include "Material.h"

#include "Shader.h"

namespace sl {

void Material::applyUniforms(Shader* shader) {
    shader->setInstanceUniforms(internalId, [&](auto self) {
        self->setUniform("diffuseColor", diffuseColor);
        self->setUniform("diffuseTexture", diffuseMap.texture);
        self->setUniform("specularTexture", specularMap.texture);
        self->setUniform("shininess", shininess);
    });
}

void Material::acquireInstanceResources() {
    internalId = shader->acquireInstanceResources();
    LOG_TRACE("{} - Acquired instance resources id = {}", name, internalId);
}

void Material::releaseInstanceResources() {
    shader->releaseInstanceResources(internalId);
    internalId = 0;
}

}  // namespace sl
