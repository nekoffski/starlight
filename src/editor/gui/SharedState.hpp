#pragma once

#include <memory>
#include <optional>

#include "GuiProperties.h"
#include "sl/ecs/Entity.h"
#include "sl/gui/Gizmo.h"
#include "sl/gui/assets/AssetGuiProvider.h"
#include "sl/scene/Scene.h"

namespace editor::gui {

struct SharedState {
    explicit SharedState(sl::asset::AssetManager& assetManager, int windowWidth, int windowHeight)
        : assetManager(assetManager),
          activeScene(nullptr),
          guiProperties(windowWidth, windowHeight),
          activeAssetGuiProvider(nullptr),
          gizmoOperation(sl::gui::GizmoOp::translate),
          gizmoSpace(sl::gui::GizmoSpace::world) {}

    bool hasSelectedEntity() const {
        return activeScene != nullptr && selectedEntityId.has_value() &&
               activeScene->ecsRegistry.hasEntityById(selectedEntityId.value());
    }

    sl::ecs::Entity& getSelectedEntity() {
        return activeScene->ecsRegistry.getEntityById(selectedEntityId.value());
    }

    sl::asset::AssetManager& assetManager;
    sl::scene::Scene* activeScene;
    std::optional<std::string> selectedEntityId;

    GuiProperties guiProperties;
    std::unique_ptr<sl::gui::assets::AssetGuiProvider> activeAssetGuiProvider;

    sl::gui::GizmoOp gizmoOperation;
    sl::gui::GizmoSpace gizmoSpace;
};
}  // namespace editor::gui