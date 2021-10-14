#include "TransformComponentGui.h"

namespace sl::gui::components {

using namespace scene::components;

void TransformComponentGui::renderComponentGuiImpl(TransformComponent& component,
    asset::AssetManager& assetManager, ecs::Entity& entity) {

    with_ID(component.ownerEntityId.c_str()) {

        if (beginComponentTreeNode(ICON_FA_STREET_VIEW "  Transform", component)) {
            m_trigerred = 0;

            ImGui::Text("Translation");
            m_trigerred += ImGui::DragFloat3("##translation", &component.position[0], 0.1f);
            ImGui::Text("Rotation");
            m_trigerred += ImGui::DragFloat3("##rotation", &component.rotation[0], 0.5f, 0.0f, 360.0f);
            ImGui::Text("Scale");
            m_trigerred += ImGui::DragFloat3("##scale", &component.scale[0], 0.5f, 0.0f, 360.0f);
            ImGui::TreePop();

            if (m_trigerred > 0)
                component.recalculateTransformation();
        }
    }
}
}