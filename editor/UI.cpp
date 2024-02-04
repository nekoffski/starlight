#include "UI.h"

#include "starlight/core/utils/Log.h"
#include "starlight/ui/fonts/FontAwesome.h"

#include "starlight/scene/components/MeshComponent.h"
#include "starlight/scene/components/TransformComponent.h"

static constexpr float leftComboWidthFactor = 0.15f;
static const sl::Vec3f selectedColor        = { 0.1f, 0.7f, 0.1f };
static const sl::Vec3f defaultColor         = { 1.0f, 1.0f, 1.0f };

UI::UI(sl::u64 w, sl::u64 h, sl::RendererFrontend& renderer, sl::Scene* scene) :
    m_width(w), m_height(h), m_scene(scene),
    m_leftCombo(
      "left-combo",
      sl::ui::PanelCombo::Properties{
        .position             = {0,                         0},
        .size                 = { leftComboWidthFactor * w, h},
        .alignWithMainMenuBar = true,
}
    ),
    m_rightCombo(
      "right-combo",
      sl::ui::PanelCombo::Properties{
        .position             = { w * (1.0f - leftComboWidthFactor), 0 },
        .size                 = { leftComboWidthFactor * w, h },
        .alignWithMainMenuBar = true,
      }
    ),
    m_shouldExit(false) {
    m_mainMenu.addMenu("File")
      .addItem("Create", []() {})
      .addItem("Open", "Ctrl+O", []() {})
      .addItem("Save", "Ctrl+S", []() {})
      .addItem("Save as", []() {})
      .addItem("Quit", [&]() { m_shouldExit = true; });
    m_mainMenu.addMenu("Help");

    m_leftCombo
      .addPanel(
        ICON_FA_CITY "  Scene",
        [&]() {
            sl::ui::treeNode(ICON_FA_PROJECT_DIAGRAM "  Scene graph", [&]() {
                m_scene->forEachEntity(
                  [&](const std::string& name, sl::Entity* entity) -> void {
                      const auto entityId = entity->getId();
                      const auto color =
                        m_selectedEntityId && m_selectedEntityId == entityId
                          ? selectedColor
                          : defaultColor;

                      sl::ui::withColor(color, [&]() {
                          if (sl::ui::text(fmt::format("{}  {}", ICON_FA_CUBE, name)
                              ))
                              m_selectedEntityId = entityId;
                      });
                  }
                );
            });
            sl::ui::separator();
            sl::ui::treeNode(ICON_FA_CLOUD "  Skybox", [&]() {

            });
            sl::ui::separator();
            sl::ui::treeNode(ICON_FA_VIDEO "  Camera", [&]() {

            });
        }
      )
      .addPanel(ICON_FA_CHART_LINE "  Statistics", [&]() {
          const auto [renderedVertices, frameNumber, delta] =
            renderer.getFrameStatistics();
          sl::ui::text("Frame time: {}", delta);
          sl::ui::text("Frames per second: {}", int(1.0f / delta));
          sl::ui::text("Frame number: {}", frameNumber);
          sl::ui::separator();
          sl::ui::text("Rendered vertices: {}", renderedVertices);
          sl::ui::separator();
          sl::ui::text("8/9/0 - change render mode");
          sl::ui::text("6     - dump var logs");
          sl::ui::text("3/4   - switch camera");
          sl::ui::text("u     - on/off update");
      });

    m_rightCombo.addPanel(ICON_FA_EYE "  Inspector", [&]() {
        if (m_selectedEntityId) {
            auto entity = m_scene->getEntity(*m_selectedEntityId);
            sl::ui::text("Entity: {}/{}", entity->getId(), entity->getName());
            sl::ui::separator();

            if (entity->hasComponent<sl::MeshComponent>()) {
                auto component = entity->getComponent<sl::MeshComponent>();
                sl::ui::treeNode(ICON_FA_PLANE "  Mesh", [&]() {
                    sl::ui::text("Geometries");
                    for (auto& geometry : component->mesh->geometries) {
                        auto properties = geometry->getProperties();
                        sl::ui::text("{}", properties.name);
                    }
                });
            }
            sl::ui::separator();
            if (entity->hasComponent<sl::TransformComponent>()) {
                auto component = entity->getComponent<sl::TransformComponent>();
                sl::ui::treeNode(ICON_FA_STREET_VIEW "  Transform", [&]() {
                    auto& transform = component->transform;
                    auto& position  = transform.getPosition();
                    auto& scale     = transform.getScale();

                    bool updated = false;

                    sl::ui::text("Translation");
                    updated |= sl::ui::slider(
                      "##Translation", position, { .min = -25.0, .max = 25.0f }
                    );

                    sl::ui::text("Scale");
                    updated |=
                      sl::ui::slider("##Scale", scale, { .min = 0.0, .max = 5.0f });

                    if (updated) transform.setAsDirty();
                });
            }
        } else {
            sl::ui::text("No entity selected");
        }
    });
}

void UI::setScene(sl::Scene* scene) { m_scene = scene; }

void UI::render() {
    m_mainMenu.render();
    m_leftCombo.render();
    m_rightCombo.render();
}

bool UI::shouldExit() const { return m_shouldExit; }
