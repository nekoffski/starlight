#include "ResourcesPanel.h"

#include "starlight/resource/All.h"

ResourcesPanel::ResourcesPanel(UIState* state, Logger* logger) :
    m_state(state), m_logger(logger), m_resourcesTab("Resources"),
    m_loadPopup("load-popup", [&]() { renderLoadPopup(); }) {
    m_resourcesTab
      .addTab(
        ICON_FA_IMAGE "  Textures",
        [&]() {
            sl::TextureManager::get().forEach(
              [&](const std::string& name, const sl::Texture* texture) {
                  auto& selectedResourceId = m_state->selectedResourceId;

                  const auto textureId = texture->getId();
                  const auto color =
                    selectedResourceId && *selectedResourceId == textureId
                      ? selectedColor
                      : defaultColor;

                  sl::ui::withColor(color, [&]() {
                      if (sl::ui::text("{}", name)) {
                          m_logger->debug(
                            "Selected resource 'Texture' with id={}", textureId
                          );
                          m_state->selectedResourceId   = textureId;
                          m_state->selectedResourceType = ResourceType::texture;
                      }
                  });
              }
            );
        }
      )
      .addTab(ICON_FA_PLANE "  Models", [&]() {})
      .addTab(ICON_FA_PROJECT_DIAGRAM "  Meshes", [&]() {})
      .addTab(ICON_FA_GLOBE_AMERICAS "  Materials", [&]() {})
      .addTab(ICON_FA_SUN "  Shaders", [&]() {});
}

void ResourcesPanel::render() {
    if (sl::ui::button("Add")) m_loadPopup.open();

    sl::ui::sameLine();
    m_resourcesTab.render();
    m_loadPopup.render();
}

void ResourcesPanel::renderLoadPopup() { sl::ui::text("Hello world!"); }
