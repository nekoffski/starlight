#pragma once

#include "Widget.h"

#include <exception>

#include "sl/asset/AssetManager.h"
#include "sl/gui/AssetsGui.hpp"
#include "sl/gui/ErrorDialog.h"
#include "sl/gui/FileBrowser.h"

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
    void render() override;

private:
    void handleCubemapLoader();
    void handleModelLoader();
    void handleTextureLoader();
    void handleShaderLoader();

    void resetArgs();

    void showLoaderPopUp();

    bool m_loadClicked;

    sl::gui::ErrorDialog m_errorDialog;
    sl::gui::FileBrowser m_fileBrowser;
    sl::gui::AssetsGui m_assetsGui;
    AssetsArgs m_assetsArgs;
};
}
