#include "ResourcesPanel.h"

ResourcesPanel::ResourcesPanel(UIState* state) :
    m_state(state), m_resourcesTab("Resources") {
    m_resourcesTab.addTab(ICON_FA_IMAGE "  Textures", [&]() {})
      .addTab(ICON_FA_PLANE "  Meshes", [&]() {})
      .addTab(ICON_FA_SUN "  Shaders", [&]() {});
}

void ResourcesPanel::render() { m_resourcesTab.render(); }
