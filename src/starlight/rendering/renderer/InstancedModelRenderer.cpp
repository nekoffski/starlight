#include <starlight/rendering/renderer/InstancedModelRenderer.h>

namespace starl::rendering::renderer {

void InstancedModelRenderer::render(const ShaderToInstancedModelRenderData& shaderToInstancedModelRenderData) {
    for (const auto& [shader, instancedModelRenderObjects] : shaderToInstancedModelRenderData) {
        shader->enable();
        shader->setUniform("projection", m_lowLevelRenderer.getProjectionMatrix());

        for (const auto& instancedModelRenderObject : instancedModelRenderObjects) {
            for (const auto& modelMatrix : instancedModelRenderObject->modelMatrices) {
                shader->setUniform("model", *modelMatrix);
                renderModel(instancedModelRenderObject->model);
            }
        }
        shader->disable();
    }
}
}
