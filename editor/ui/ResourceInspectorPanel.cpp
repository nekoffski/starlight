#include "ResourceInspectorPanel.h"

#include "starlight/scene/components/All.h"
#include "starlight/resource/All.h"

#include "backends/imgui_impl_vulkan.h"

ResourceInspectorPanel::ResourceInspectorPanel(
  sl::Scene* scene, UIState* state, Logger* logger
) :
    m_scene(scene),
    m_state(state), m_logger(logger) {}

void ResourceInspectorPanel::render() {
    if (m_state->selectedResourceId) {
        renderResourceUI(
          *m_state->selectedResourceId, *m_state->selectedResourceType
        );
    } else {
        sl::ui::text("No resource selected");
    }
}

void ResourceInspectorPanel::renderResourceUI(
  sl::u64 resourceId, ResourceType type
) {
    switch (type) {
        case ResourceType::texture:
            return renderTextureUI(sl::TextureManager::get().acquire(resourceId));
    }
}

void ResourceInspectorPanel::renderTextureUI(sl::Texture* texture) {
    sl::ui::namedScope("texture-resource-panel", [&]() {
        const auto props = texture->getProperties();
        sl::ui::text("Texture");
        sl::ui::separator();
        sl::ui::text("{}", props.name);
        sl::ui::text("Size: {}x{}", props.width, props.height);
        sl::ui::text("Channels: {}", props.channels);
        sl::ui::text("Transparent: {}", props.isTransparent);
        sl::ui::text("Writable: {}", props.isWritable);

        // auto imageHandle = ImGui::

        // ImGui::Image()
    });
}
