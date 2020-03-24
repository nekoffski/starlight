#pragma once

#include <memory>
#include <unordered_map>

#include <starlight/framework/graphics/LowLevelRenderer.h>
#include <starlight/platform/shader/Shader.h>
#include <starlight/rendering/renderable/InstancedModelRenderObject.h>
#include <starlight/rendering/renderer/ModelRenderer.h>

// TODO: create FWD
namespace starl::rendering::renderer {

using ShaderPtr = std::shared_ptr<shader::Shader>;
using InstancedModelRenderObjectPtr = std::shared_ptr<renderable::InstancedModelRenderObject>;
using ShaderToInstancedModelRenderObjects = std::unordered_map<ShaderPtr, std::vector<InstancedModelRenderObjectPtr>>;

class InstancedModelRenderer : public ModelRenderer {
public:
    using ModelRenderer::ModelRenderer;

    void render() override;

    void pushInstancedModelRenderObject(ShaderPtr& shader, InstancedModelRenderObjectPtr& instancedModelRenderObject) {
        m_shaderToInstancedModelRenderObjects[shader].push_back(instancedModelRenderObject);
    }

private:
    ShaderToInstancedModelRenderObjects m_shaderToInstancedModelRenderObjects;
};
}