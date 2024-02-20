#include "EntityInspectorPanel.h"

#include "starlight/scene/components/All.h"
#include "starlight/resource/All.h"

EntityInspectorPanel::EntityInspectorPanel(
  sl::Scene* scene, UIState* state, Logger* logger
) :
    m_scene(scene),
    m_state(state), m_logger(logger),
    m_translationSlider("Translation", { .min = -15.0, .max = 15.0f, .step = 0.1f }),
    m_scaleSlider(
      "Scale", { .min = 0.0, .max = 5.0f, .step = 0.1f }, sl::Vec3f{ 1.0f }
    ),
    m_orientationSlider(
      "Orientation", { .min = -3.14, .max = 3.14, .step = 0.01f }
    ) {}

void EntityInspectorPanel::render() {
    if (m_state->selectedEntityId)
        renderEntityUI(*m_state->selectedEntityId);
    else
        sl::ui::text("No entity selected");
}

void EntityInspectorPanel::renderEntityUI(sl::u64 entityId) {
    auto entity = m_scene->getEntity(entityId);
    sl::ui::text("Entity: {}/{}", entity->getId(), entity->getName());
    sl::ui::separator();

    if (entity->hasComponent<sl::ModelComponent>()) {
        auto component = entity->getComponent<sl::ModelComponent>();
        sl::ui::treeNode(ICON_FA_PLANE "  Model", [&]([[maybe_unused]] bool) {
            sl::ui::text("Meshes");
            component->model->forEachMesh([&](sl::Mesh* mesh) {
                auto properties = mesh->getProperties();
                sl::ui::text("{}", properties.name);
            });
        });
        sl::ui::separator();
    }

    if (entity->hasComponent<sl::TransformComponent>()) {
        auto component = entity->getComponent<sl::TransformComponent>();
        sl::ui::treeNode(
          ICON_FA_STREET_VIEW "  Transform",
          [&]([[maybe_unused]] bool) {
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
          }
        );
        sl::ui::separator();
    }
}
