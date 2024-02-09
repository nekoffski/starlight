#include "ResourcesPanel.h"

#include "starlight/resource/ResourceManager.h"

ResourcesPanel::ResourcesPanel(UIState* state, Logger* logger) :
    m_state(state), m_logger(logger), m_resourcesTab("Resources") {
    m_resourcesTab
      .addTab(
        ICON_FA_IMAGE "  Textures",
        [&]() {
            sl::ResourceManager::get().forEachTexture(
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
      .addTab(ICON_FA_PLANE "  Meshes", [&]() {})
      .addTab(ICON_FA_SUN "  Shaders", [&]() {});
}

void ResourcesPanel::render() { m_resourcesTab.render(); }
