#pragma once

#include "Widget.h"

namespace editor::gui {

class ToolBar : public Widget {
public:
    explicit ToolBar(std::shared_ptr<SharedState> sharedState)
        : Widget(sharedState)
        , m_gizmoOperation(0) {
    }

    void render(sl::gui::GuiApi& gui) {
        auto& properties = m_sharedState->guiProperties.toolBarProperties;
        gui.beginTransparentPanel("Toolbar", properties.origin, properties.size);
        gui.pushId("ToolBar");

        if (gui.button(ICON_FA_PLAY)) {
        }

        gui.sameLine();

        if (gui.button(ICON_FA_PAUSE)) {
        }

        gui.sameLine();

        if (gui.button(ICON_FA_STOP)) {
        }

        gui.sameLine();

        gui.displayText("  " ICON_FA_ELLIPSIS_V "  ");

        gui.sameLine();

        auto handleGizmoOperation = [&](const std::string& label, int index) {
            bool isSelected = (m_gizmoOperation == index);

            if (isSelected)
                gui.pushTextColor(sl::gui::selectedEntryColor);

            if (gui.button(label)) {
                m_gizmoOperation = index;
                m_sharedState->gizmoOperation = getGizmoOperation();
            }

            if (isSelected)
                gui.popColor();
        };

        handleGizmoOperation(ICON_FA_ARROWS_ALT, 0);
        gui.sameLine();

        handleGizmoOperation(ICON_FA_SYNC_ALT, 1);
        gui.sameLine();

        handleGizmoOperation(ICON_FA_EXPAND_ALT, 2);

        gui.popId();
        gui.endPanel();
    }

private:
    sl::gui::GizmoOperation getGizmoOperation() {
        switch (m_gizmoOperation) {
        case 0:
            return sl::gui::GizmoOperation::translate;

        case 1:
            return sl::gui::GizmoOperation::rotate;

        case 2:
            return sl::gui::GizmoOperation::scale;
        }
        return sl::gui::GizmoOperation::translate;
    }

    int m_gizmoOperation;
};

}