#include "TransformComponentGui.h"

namespace sl::gui::components {

using namespace scene::components;

void TransformComponentGui::renderComponentGuiImpl(TransformComponent& component,
    gui::GuiApi& gui, asset::AssetManager& assetManager, ecs::Entity& entity) {

    gui.pushId(component.ownerEntityId);

    if (beginComponentTreeNode(gui, ICON_FA_STREET_VIEW "  Transform", component)) {
        m_trigerred = 0;

        gui.displayText("Translation");
        m_trigerred += gui.dragFloat3(gui::createHiddenLabel("translation"), component.position, 0.1f);
        gui.displayText("Rotation");
        m_trigerred += gui.dragFloat3(gui::createHiddenLabel("rotation"), component.rotation, 0.5f, 0.0f, 360.0f);
        gui.displayText("Scale");
        m_trigerred += gui.dragFloat3(gui::createHiddenLabel("scale"), component.scale, 0.5f, 0.0f, 360.0f);
        gui.popTreeNode();

        if (m_trigerred > 0)
            component.recalculateTransformation();
    }

    gui.popId();
}
}