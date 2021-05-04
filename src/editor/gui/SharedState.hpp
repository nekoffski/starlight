#pragma once

#include <memory>

#include "GuiProperties.h"
#include "sl/ecs/Entity.h"
#include "sl/gui/assets/AssetGuiProvider.h"
#include "sl/scene/Scene.h"

namespace editor::gui {

struct SharedState {
    explicit SharedState(sl::asset::AssetManager& assetManager, int windowWidth, int windowHeight)
        : assetManager(assetManager)
        , guiProperties(windowWidth, windowHeight)
        , activeAssetGuiProvider(nullptr) {
    }

    sl::asset::AssetManager& assetManager;
    std::weak_ptr<sl::scene::Scene> activeScene;
    std::weak_ptr<sl::ecs::Entity> selectedEntity;
    GuiProperties guiProperties;
    std::unique_ptr<sl::gui::assets::AssetGuiProvider> activeAssetGuiProvider;
};
}