#pragma once

#include <memory>

#include "GuiProperties.h"
#include "sl/ecs/Entity.h"
#include "sl/scene/Scene.h"

namespace editor::gui {

struct SharedState {
    explicit SharedState(sl::asset::AssetManager& assetManager, int windowWidth, int windowHeight)
        : assetManager(assetManager)
        , guiProperties(windowWidth, windowHeight) {
    }

    sl::asset::AssetManager& assetManager;
    std::weak_ptr<sl::scene::Scene> activeScene;
    std::shared_ptr<sl::ecs::Entity> selectedEntity;
    GuiProperties guiProperties;
};
}