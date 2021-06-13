#pragma once

#include "Widget.h"

#include "editor/EngineState.h"
#include "editor/Events.h"

namespace editor::gui {

class ToolBar : public Widget {
public:
    explicit ToolBar(std::shared_ptr<SharedState> sharedState)
        : Widget(sharedState)
        , m_gizmoOperation(sl::gui::GizmoOperation::translate)
        , m_engineState(EngineState::stopped) {
    }

    void render(sl::gui::GuiApi& gui) {
        auto& properties = m_sharedState->guiProperties.toolBarProperties;

        gui.beginTransparentPanel("Toolbar", properties.origin, properties.size);
        gui.pushId("ToolBar");

        auto handleEngineStateButton = [&](const std::string& label, EngineState state) -> void {
            bool isSelected = m_engineState == state;

            if (isSelected)
                gui.pushTextColor(sl::gui::selectedEntryColor);

            if (gui.button(label)) {
                sl::event::Emitter::emit<EngineStateChanged>(state);
                m_engineState = state;
            }

            if (isSelected)
                gui.popColor();
        };

        if (gui.button(ICON_FA_GAMEPAD))
            sl::event::Emitter::emit<EnterGameMode>();

        gui.sameLine();
        gui.displayText("  " ICON_FA_ELLIPSIS_V "  ");
        gui.sameLine();

        handleEngineStateButton(ICON_FA_PLAY, EngineState::started);
        gui.sameLine();

        handleEngineStateButton(ICON_FA_PAUSE, EngineState::paused);
        gui.sameLine();

        handleEngineStateButton(ICON_FA_STOP, EngineState::stopped);
        gui.sameLine();

        gui.displayText("  " ICON_FA_ELLIPSIS_V "  ");

        gui.sameLine();

        auto handleGizmoOperation = [&](const std::string& label, sl::gui::GizmoOperation operation) -> void {
            bool isSelected = (m_gizmoOperation == operation);

            if (isSelected)
                gui.pushTextColor(sl::gui::selectedEntryColor);

            if (gui.button(label)) {
                m_gizmoOperation = operation;
                m_sharedState->gizmoOperation = operation;
            }

            if (isSelected)
                gui.popColor();
        };

        handleGizmoOperation(ICON_FA_ARROWS_ALT, sl::gui::GizmoOperation::translate);
        gui.sameLine();

        handleGizmoOperation(ICON_FA_SYNC_ALT, sl::gui::GizmoOperation::rotate);
        gui.sameLine();

        handleGizmoOperation(ICON_FA_EXPAND_ALT, sl::gui::GizmoOperation::scale);

        gui.popId();
        gui.endPanel();
    }

private:
    sl::gui::GizmoOperation m_gizmoOperation;
    EngineState m_engineState;
};

}