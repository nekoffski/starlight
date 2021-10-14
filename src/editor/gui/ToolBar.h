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

    void render() {
        auto& properties = m_sharedState->guiProperties.toolBarProperties;

        sl::gui::beginTransparentPanel("Toolbar", properties.origin, properties.size);

        with_ID("ToolBar") {

            auto handleEngineStateButton = [&](const std::string& label, EngineState state) -> void {
                bool isSelected = m_engineState == state;

                if (isSelected)
                    sl::gui::pushTextColor(sl::gui::selectedEntryColor);

                if (ImGui::Button(label.c_str())) {
                    sl::event::EventManager::get()->emit<EngineStateChanged>(state).toAll();
                    m_engineState = state;
                }

                if (isSelected)
                    sl::gui::popTextColor();
            };

            if (ImGui::Button(ICON_FA_GAMEPAD))
                sl::event::EventManager::get()->emit<EnterGameMode>().toAll();

            ImGui::SameLine();
            ImGui::Text("  " ICON_FA_ELLIPSIS_V "  ");
            ImGui::SameLine();

            handleEngineStateButton(ICON_FA_PLAY, EngineState::started);
            ImGui::SameLine();

            handleEngineStateButton(ICON_FA_PAUSE, EngineState::paused);
            ImGui::SameLine();

            handleEngineStateButton(ICON_FA_STOP, EngineState::stopped);
            ImGui::SameLine();

            ImGui::Text("  " ICON_FA_ELLIPSIS_V "  ");
            ImGui::SameLine();

            auto handleGizmoOperation = [&](const std::string& label, sl::gui::GizmoOperation operation) -> void {
                bool isSelected = (m_gizmoOperation == operation);

                if (isSelected)
                    sl::gui::pushTextColor(sl::gui::selectedEntryColor);

                if (ImGui::Button(label.c_str())) {
                    m_gizmoOperation = operation;
                    m_sharedState->gizmoOperation = operation;
                }

                if (isSelected)
                    sl::gui::popTextColor();
            };

            handleGizmoOperation(ICON_FA_ARROWS_ALT, sl::gui::GizmoOperation::translate);
            ImGui::SameLine();

            handleGizmoOperation(ICON_FA_SYNC_ALT, sl::gui::GizmoOperation::rotate);
            ImGui::SameLine();

            handleGizmoOperation(ICON_FA_EXPAND_ALT, sl::gui::GizmoOperation::scale);
        }
        sl::gui::endPanel();
    }

private:
    sl::gui::GizmoOperation m_gizmoOperation;
    EngineState m_engineState;
};

}