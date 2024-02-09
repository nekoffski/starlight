#include "ScenePanel.h"

#include "starlight/ui/UI.h"

ScenePanel::ScenePanel(sl::Scene* scene, UIState* state, Logger* logger) :
    m_scene(scene), m_state(state), m_logger(logger), m_tabs("scene-tabs") {
    m_tabs
      .addTab(
        ICON_FA_PROJECT_DIAGRAM "  Scene graph",
        [&]() {
            if (sl::ui::button("Add Entity")) {
                m_logger->debug("Adding new entity");
            }
            sl::ui::separator();
            sl::ui::treeNode(ICON_FA_CUBE "  Root", [&]() {
                m_scene->forEachEntity(
                  [&](const std::string& name, sl::Entity* entity) -> void {
                      auto& selectedEntityId = m_state->selectedEntityId;

                      const auto entityId = entity->getId();
                      const auto color =
                        selectedEntityId && *selectedEntityId == entityId
                          ? selectedColor
                          : defaultColor;

                      sl::ui::withColor(color, [&]() {
                          const auto title =
                            fmt::format("\t{}  {}", ICON_FA_CUBE, name);
                          if (sl::ui::text(title)) {
                              m_logger->debug(
                                "Selected entity with id={}", entityId
                              );
                              selectedEntityId = entityId;
                          }
                      });
                  }
                );
            });
        }
      )
      .addTab(ICON_FA_CLOUD "  Skybox", [&]() {});
}

void ScenePanel::render() { m_tabs.render(); }
