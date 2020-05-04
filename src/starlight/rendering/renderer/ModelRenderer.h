#pragma once

#include <memory>
#include <unordered_map>

#include <starlight/framework/graphics/LowLevelRenderer.h>
#include <starlight/platform/shader/Shader.h>
#include <starlight/rendering/data/ModelRenderData.h>
#include <starlight/rendering/renderer/Renderer.h>

// TODO: create FWD
namespace starl::rendering::renderer {

using ShaderPtr = std::shared_ptr<platform::shader::Shader>;
using ModelRenderDataPtr = std::shared_ptr<data::ModelRenderData>;
using ShaderToModelRenderData = std::unordered_map<ShaderPtr, std::vector<ModelRenderDataPtr>>;

class ModelRenderer : public Renderer {
public:
    using Renderer::Renderer;

    void render(const ShaderToModelRenderData&, const std::shared_ptr<framework::graphics::camera::Camera>&);

protected:
    void renderModel(const std::shared_ptr<geometry::Model>&);
    void renderMesh(const std::shared_ptr<geometry::Mesh>&);
};
}