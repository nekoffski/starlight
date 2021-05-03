#pragma once

#include <memory>

#include "GuiProperties.h"
#include "sl/ecs/Entity.h"
#include "sl/gui/assets/AssetGui.h"
#include "sl/scene/Scene.h"

namespace editor::gui {

struct SharedState {
    explicit SharedState(sl::asset::AssetManager& assetManager, int windowWidth, int windowHeight)
        : assetManager(assetManager)
        , guiProperties(windowWidth, windowHeight)
        , activeAssetGui(nullptr) {
    }

    sl::asset::AssetManager& assetManager;
    std::weak_ptr<sl::scene::Scene> activeScene;
    std::weak_ptr<sl::ecs::Entity> selectedEntity;
    GuiProperties guiProperties;
    std::unique_ptr<sl::gui::assets::AssetGui> activeAssetGui;
};
}