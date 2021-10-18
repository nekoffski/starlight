#pragma once

#include <memory>
#include <optional>

#include "GuiProperties.h"
#include "sl/ecs/Entity.h"

#include "sl/gui/assets/AssetGuiProvider.h"
#include "sl/scene/Scene.h"

namespace editor::gui {

struct SharedState {
    explicit SharedState(sl::asset::AssetManager& assetManager, int windowWidth, int windowHeight)
        : assetManager(assetManager)
        , guiProperties(windowWidth, windowHeight)
        , activeAssetGuiProvider(nullptr)
        , gizmoOperation(sl::gui::GizmoOperation::translate)
        , gizmoSystem(sl::gui::GizmoSystem::world) {
    }

    bool hasSelectedEntity() const {
        auto scene = activeScene.lock();

        if (not scene)
            return false;

        return selectedEntityId.has_value() &&
            scene->ecsRegistry.hasEntityById(selectedEntityId.value());
    }

    sl::ecs::Entity& getSelectedEntity() {
        return activeScene.lock()->ecsRegistry.getEntityById(selectedEntityId.value());
    }

    sl::asset::AssetManager& assetManager;
    std::weak_ptr<sl::scene::Scene> activeScene;
    std::optional<std::string> selectedEntityId;

    GuiProperties guiProperties;
    std::unique_ptr<sl::gui::assets::AssetGuiProvider> activeAssetGuiProvider;
    sl::gui::GizmoOperation gizmoOperation;
    sl::gui::GizmoSystem gizmoSystem;
};
}