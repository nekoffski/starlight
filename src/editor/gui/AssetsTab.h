#pragma once

#include "Widget.h"

#include <exception>

#include "sl/asset/AssetManager.h"
#include "sl/gui/ErrorDialog.hpp"
#include "sl/gui/FileBrowser.hpp"
#include "sl/gui/GuiApi.h"

namespace editor::gui {

class AssetsTab : public Widget {
    struct AssetsArgs {
        std::string assetName;
        std::string modelName;
        int activeItem;
        std::array<std::string, 6> faces;
    };

public:
    explicit AssetsTab(std::shared_ptr<SharedState>);
    void render(sl::gui::GuiApi& gui) override;

private:
    void handleCubemapLoader(sl::gui::GuiApi& gui);
    void handleModelLoader(sl::gui::GuiApi& gui);
    void handleTextureLoader(sl::gui::GuiApi& gui);
    void handleShaderLoader(sl::gui::GuiApi& gui);

    void resetArgs();

    void showLoaderPopUp(sl::gui::GuiApi& gui);

    bool m_loadClicked;

    sl::gui::ErrorDialog m_errorDialog;
    sl::gui::FileBrowser m_fileBrowser;
    AssetsArgs m_assetsArgs;
};
}
