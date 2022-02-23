#pragma once

#include <memory>

#include "AssetTab.h"
#include "SystemTab.h"
#include "Widget.h"
#include "sl/asset/AssetManager.h"
#include "sl/ecs/Entity.h"
#include "sl/gfx/Cubemap.h"

namespace editor::gui {

class PropertiesPanel : public Widget {
   public:
    explicit PropertiesPanel(std::shared_ptr<SharedState> sharedState);
    void render() override;

   private:
    void showSceneProperties();

    sl::gfx::Cubemap* m_selectedCubemap = nullptr;

    AssetTab m_assetTab;
    SystemTab m_systemTab;
};
}  // namespace editor::gui
