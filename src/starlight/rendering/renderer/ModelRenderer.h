#pragma once

#include <memory>
#include <unordered_map>

#include <starlight/framework/graphics/LowLevelRenderer.h>
#include <starlight/platform/shader/Shader.h>
#include <starlight/rendering/renderable/ModelRenderObject.h>
#include <starlight/rendering/renderer/Renderer.h>

// TODO: create FWD
namespace starl::rendering::renderer {

using ShaderPtr = std::shared_ptr<shader::Shader>;
using ModelRenderObjectPtr = std::shared_ptr<renderable::ModelRenderObject>;
using ShaderToModelRenderObjects = std::unordered_map<ShaderPtr, std::vector<ModelRenderObjectPtr>>;

class ModelRenderer : public Renderer {
public:
    using Renderer::Renderer;

    void render() override;
    void pushModelRenderObject(ShaderPtr& shader, ModelRenderObjectPtr& modelRenderObject) {
        m_shaderToModelRenderObjects[shader].push_back(modelRenderObject);
    }

protected:
    void renderModel(const std::shared_ptr<geometry::Model>&);
    void renderMesh(const std::shared_ptr<geometry::Mesh>&);

private:
    ShaderToModelRenderObjects m_shaderToModelRenderObjects;
};
}