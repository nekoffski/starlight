#pragma once

#include "Widget.h"

#include "editor/res/ResourceManager.h"
#include "sl/gui/GuiProxy.h"

namespace editor::gui {

class ResourcesTab : public Widget {
public:
    explicit ResourcesTab(res::ResourceManager& resourceManager);
    void render(sl::gui::GuiProxy& gui) override;

private:
    void handleCubemapLoader(sl::gui::GuiProxy& gui);
	void handleModelLoader(sl::gui::GuiProxy& gui);
	void handleTextureLoader(sl::gui::GuiProxy& gui);

	void showLoaderPopUp(sl::gui::GuiProxy& gui);

    res::ResourceManager& m_resourceManager;
    bool m_loadClicked;

    std::string m_resourceName;
};
}
