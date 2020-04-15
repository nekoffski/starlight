#pragma once

#include <memory>
#include <unordered_map>

#include <starlight/framework/graphics/LowLevelRenderer.h>
#include <starlight/platform/shader/Shader.h>
#include <starlight/rendering/entity/ModelRenderEntity.h>
#include <starlight/rendering/renderer/Renderer.h>

// TODO: create FWD
namespace starl::rendering::renderer {

using ShaderPtr = std::shared_ptr<shader::Shader>;
using ModelRenderEntityPtr = std::shared_ptr<entity::ModelRenderEntity>;
using ShaderToModelRenderEntities = std::unordered_map<ShaderPtr, std::vector<ModelRenderEntityPtr>>;

class ModelRenderer : public Renderer {
public:
    using Renderer::Renderer;

    void render(const ShaderToModelRenderEntities&);

protected:
    void renderModel(const std::shared_ptr<geometry::Model>&);
    void renderMesh(const std::shared_ptr<geometry::Mesh>&);
};
}