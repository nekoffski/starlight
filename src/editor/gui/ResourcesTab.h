#pragma once

#include "Widget.h"

#include "editor/res/ResourceManager.h"
#include "sl/gui/GuiApi.h"

namespace editor::gui {

class ResourcesTab : public Widget {
public:
    explicit ResourcesTab(res::ResourceManager& resourceManager);
    void render(sl::gui::GuiApi& gui) override;

private:
    void handleCubemapLoader(sl::gui::GuiApi& gui);
    void handleModelLoader(sl::gui::GuiApi& gui);
    void handleTextureLoader(sl::gui::GuiApi& gui);

    void showLoaderPopUp(sl::gui::GuiApi& gui);

    res::ResourceManager& m_resourceManager;
    bool m_loadClicked;

    std::string m_resourceName;
};
}
