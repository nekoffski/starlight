#include "InspectorPanel.h"

#include "starlight/scene/components/All.h"
#include "starlight/resource/ResourceManager.h"

InspectorPanel::InspectorPanel(sl::Scene* scene, UIState* state, Logger* logger) :
    m_scene(scene), m_state(state), m_logger(logger), m_menu("Inspector"),
    m_translationSlider("Translation", { .min = -15.0, .max = 15.0f, .step = 0.1f }),
    m_scaleSlider(
      "Scale", { .min = 0.0, .max = 5.0f, .step = 0.1f }, sl::Vec3f{ 1.0f }
    ),
    m_orientationSlider(
      "Orientation", { .min = -3.14, .max = 3.14, .step = 0.01f }
    ) {
    m_menu
      .addTab(
        ICON_FA_CUBE "  Entity",
        [&]() {
            if (m_state->selectedEntityId)
                renderEntityUI(*m_state->selectedEntityId);
            else
                sl::ui::text("No entity selected");
        }
      )
      .addTab(ICON_FA_FILE "  Resource", [&]() {
          if (m_state->selectedResourceId) {
              renderResourceUI(
                *m_state->selectedResourceId, *m_state->selectedResourceType
              );
          } else {
              sl::ui::text("No resource selected");
          }
      });
}

void InspectorPanel::render() { m_menu.render(); }

void InspectorPanel::renderEntityUI(sl::u64 entityId) {
    auto entity = m_scene->getEntity(entityId);
    sl::ui::text("Entity: {}/{}", entity->getId(), entity->getName());
    sl::ui::separator();

    if (entity->hasComponent<sl::ModelComponent>()) {
        auto component = entity->getComponent<sl::ModelComponent>();
        sl::ui::treeNode(ICON_FA_PLANE "  Mesh", [&]() {
            sl::ui::text("Geometries");
            for (auto& geometry : component->model->meshes) {
                auto properties = geometry->getProperties();
                sl::ui::text("{}", properties.name);
            }
        });
        sl::ui::separator();
    }

    if (entity->hasComponent<sl::TransformComponent>()) {
        auto component = entity->getComponent<sl::TransformComponent>();
        sl::ui::treeNode(ICON_FA_STREET_VIEW "  Transform", [&]() {
            auto& transform = component->transform;

            sl::ui::namedScope("transform-component-ui", [&]() {
                m_translationSlider.render([&](const sl::Vec3f& data) {
                    transform.setPosition(data);
                });

                m_scaleSlider.render([&](const sl::Vec3f& data) {
                    transform.setScale(data);
                });

                m_orientationSlider.render([&](const sl::Vec3f& data) {
                    transform.setRotation(sl::math::orientate4(data));
                });
            });
        });
        sl::ui::separator();
    }
}

void InspectorPanel::renderResourceUI(sl::u64 resourceId, ResourceType type) {
    auto resourceManager = sl::ResourceManager::getPtr();

    switch (type) {
        case ResourceType::texture:
            return renderTextureUI(resourceManager->acquireTexture(resourceId));
    }
}

void InspectorPanel::renderTextureUI(sl::Texture* texture) {
    sl::ui::namedScope("texture-resource-panel", [&]() {
        const auto props = texture->getProperties();
        sl::ui::text("Texture");
        sl::ui::separator();
        sl::ui::text("{}", props.name);
        sl::ui::text("Size: {}x{}", props.width, props.height);
        sl::ui::text("Channels: {}", props.channels);
        sl::ui::text("Transparent: {}", props.isTransparent);
        sl::ui::text("Writable: {}", props.isWritable);
    });
}
