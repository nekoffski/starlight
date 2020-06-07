#pragma once

#include <memory>
#include <unordered_map>

#include <starlight/core/log/Logger.h>
#include <starlight/framework/graphics/LowLevelRenderer.h>
#include <starlight/platform/gpu/ElementBuffer.h>
#include <starlight/platform/gpu/VertexArray.h>
#include <starlight/platform/gpu/VertexBuffer.h>
#include <starlight/platform/shader/Shader.h>
#include <starlight/rendering/data/ModelRenderData.h>
#include <starlight/rendering/renderer/Renderer.h>

namespace starl::rendering::renderer {

// using ShaderPtr = std::shared_ptr<platform::shader::Shader>;
// using ModelRenderDataPtr = std::shared_ptr<data::ModelRenderData>;
// using ShaderToModelRenderData = std::unordered_map<ShaderPtr, std::vector<ModelRenderDataPtr>>;

class ParticleRenderer : public Renderer {
public:
    explicit ParticleRenderer(framework::graphics::LowLevelRenderer& lowLevelRenderer, std::shared_ptr<platform::shader::Shader> particleShader)
        : Renderer(lowLevelRenderer)
        , m_particleShader(std::move(particleShader)) {
        m_particleVertexArray = platform::gpu::VertexArray::create();

        float vertices[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f
        };

        auto vbo = platform::gpu::VertexBuffer::create(vertices, sizeof(vertices), 4);

        int indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        auto ebo = platform::gpu::ElementBuffer::create(indices, sizeof(indices), 2);

        m_particleVertexArray->addVertexBuffer(vbo);
        m_particleVertexArray->addElementBuffer(ebo);
    }

    void render() {
    }

private:
    std::shared_ptr<platform::shader::Shader> m_particleShader;
    std::shared_ptr<platform::gpu::VertexArray> m_particleVertexArray;
};
}