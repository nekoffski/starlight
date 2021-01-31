#pragma once

#include "Widget.h"

#include "editor/res/ResourceManager.h"
#include "sl/gui/ErrorDialog.hpp"
#include "sl/gui/GuiApi.h"

namespace editor::gui {

class ResourcesTab : public Widget {
    struct ResourcesArgs {
        std::string resourceName;
	    std::string modelName; 
		int activeItem;
		std::array<std::string, 6> faces; 
	};

public:
    explicit ResourcesTab(res::ResourceManager& resourceManager);
    void render(sl::gui::GuiApi& gui) override;

private:
    void handleCubemapLoader(sl::gui::GuiApi& gui);
    void handleModelLoader(sl::gui::GuiApi& gui);
    void handleTextureLoader(sl::gui::GuiApi& gui);

    void resetArgs();

    void showLoaderPopUp(sl::gui::GuiApi& gui);

    res::ResourceManager& m_resourceManager;
    bool m_loadClicked;

    sl::gui::ErrorDialog m_errorDialog;

	ResourcesArgs m_resourcesArgs;
};
}
