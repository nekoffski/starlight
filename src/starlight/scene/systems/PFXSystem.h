#pragma once

#include "starlight/asset/AssetManager.hpp"
#include "starlight/platform/shader/Shader.h"
#include "starlight/rendering/RendererProxy.h"
#include "starlight/rendering/camera/Camera.h"
#include "starlight/scene/components/PFXComponent.h"

namespace sl::scene::systems {

class PFXSystem {
public:
    explicit PFXSystem(std::shared_ptr<rendering::RendererProxy> renderer)
        : m_renderer(renderer) {
        m_shader = asset::AssetManager::load<platform::shader::Shader>(
            "/particle.vert", "/particle.frag");
    }

    void renderPFXs(std::vector<components::PFXComponent>& pfxs, std::shared_ptr<rendering::camera::Camera> camera) {
        for (auto& pfx : pfxs) {
            m_shader->enable();
            m_shader->setUniform("view", camera->getViewMatrix());
            m_shader->setUniform("viewPos", camera->getPosition());
            m_shader->setUniform("model", math::translate(pfx.position));

            m_renderer->beginPFX(m_shader);

            // m_renderer->endPFX();
            m_shader->disable();
        }
    }

private:
    std::shared_ptr<platform::shader::Shader> m_shader;
    std::shared_ptr<rendering::RendererProxy> m_renderer;
};
}
