#include "ScenePanel.h"

#include "starlight/ui/UI.h"

ScenePanel::ScenePanel(sl::Scene* scene, UIState* state) :
    m_scene(scene), m_state(state) {}

void ScenePanel::render() {
    if (ImGui::BeginTabBar("Scene")) {
        if (ImGui::BeginTabItem(ICON_FA_PROJECT_DIAGRAM "  Scene graph")) {
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
                          if (sl::ui::text(title)) selectedEntityId = entityId;
                      });
                  }
                );
            });
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(ICON_FA_CLOUD "  Skybox")) {
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}
