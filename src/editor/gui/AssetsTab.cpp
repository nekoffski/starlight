#include "AssetsTab.h"

#include "editor/DebugConsole.hpp"
#include "sl/asset/AssetLoader.hpp"
#include "sl/core/error/Errors.hpp"
#include "sl/core/utils/String.hpp"
#include "sl/gui/GuiApi.h"
#include "sl/gui/Utils.hpp"

namespace editor::gui {

constexpr int padding = 65;

static void validateAssetName(const std::string& name) {
    using namespace sl::core::error;

    if (sl::core::utils::isStringEmpty(name))
        throw AssetError(ErrorCode::AssetError, "Asset name cannot be empty");
}

AssetsTab::AssetsTab(std::shared_ptr<SharedState> sharedState)
    : Widget(sharedState)
    , m_fileBrowser("Assets file browser") {
}

void AssetsTab::render(sl::gui::GuiApi& gui) {
    gui.beginGroup();
    if (gui.button("+")) {
        resetArgs();
        gui.openPopUp("AssetLoadPopUp");
    }

    if (gui.beginPopUp("AssetLoadPopUp")) {
        showLoaderPopUp(gui);
        gui.endPopUp();
    }

    gui.endGroup();

    for (auto& [_, namesVector] : m_sharedState->assetManager.getAssetsNames())
        for (auto& assetName : namesVector)
            gui.displayText(assetName);
}

void AssetsTab::resetArgs() {
    m_assetsArgs.assetName = "";
    m_assetsArgs.activeItem = 0;
    m_assetsArgs.modelName = "/tow/tower.obj";
    m_assetsArgs.faces = {
        "/skybox/right.jpg", "/skybox/left.jpg", "/skybox/top.jpg", "/skybox/bottom.jpg", "/skybox/front.jpg", "/skybox/back.jpg"
    };
}

void AssetsTab::showLoaderPopUp(sl::gui::GuiApi& gui) {
    static const std::vector<std::string> labels = { "Cubemap", "Texture", "Model", "Shader" };

    gui.combo(sl::gui::createHiddenLabel("AssetsCombo"), m_assetsArgs.activeItem, labels);

    m_loadClicked = false;

    if (gui.button("Quit"))
        gui.closeCurrentPopUp();

    gui.sameLine();
    if (gui.button("Load")) {
        m_loadClicked = true;
    }

    gui.sameLine(250);
    gui.beginGroup();

    gui.displayText("Name");
    gui.sameLine(padding);
    gui.inputText(sl::gui::createHiddenLabel("Name"), m_assetsArgs.assetName);

    try {
        switch (m_assetsArgs.activeItem) {
        case 0: {
            handleCubemapLoader(gui);
            break;
        }

        case 1: {
            handleTextureLoader(gui);
            break;
        }

        case 2: {
            handleModelLoader(gui);
            break;
        }

        case 3: {
            handleShaderLoader(gui);
            break;
        }
        }
    } catch (sl::core::error::AssetError& err) {
        m_errorDialog.setErrorMessage(err.getDetails());
        WRITE_DEBUG(err.as<std::string>());
        m_loadClicked = false;
    }

    if (m_loadClicked) {
        gui.closeCurrentPopUp();
    }

    gui.endGroup();
    m_errorDialog.show(gui);
}

void AssetsTab::handleShaderLoader(sl::gui::GuiApi& gui) {
    sl::gui::labeledTextInput(gui, "Vertex shader", m_assetsArgs.faces[0], padding);
    if (gui.isPreviousWidgetClicked())
        m_fileBrowser.open(&m_assetsArgs.faces[0], gui);

    sl::gui::labeledTextInput(gui, "Fragment shader", m_assetsArgs.faces[1], padding);
    if (gui.isPreviousWidgetClicked())
        m_fileBrowser.open(&m_assetsArgs.faces[1], gui);

    sl::gui::labeledTextInput(gui, "Geometry shader", m_assetsArgs.faces[2], padding);
    if (gui.isPreviousWidgetClicked())
        m_fileBrowser.open(&m_assetsArgs.faces[2], gui);

    m_fileBrowser.show(gui);

    if (m_loadClicked) {
        validateAssetName(m_assetsArgs.assetName);

        auto shader = sl::asset::AssetLoader::loadGlobalPath<sl::graphics::Shader>(
            m_assetsArgs.faces[0], m_assetsArgs.faces[1], m_assetsArgs.faces[2]);
        auto shaderAsset = std::make_shared<sl::asset::ShaderAsset>(shader, m_assetsArgs.assetName);
        m_sharedState->assetManager.addAsset(shaderAsset);
    }
}

void AssetsTab::handleCubemapLoader(sl::gui::GuiApi& gui) {
    sl::gui::labeledTextInput(gui, "Right", m_assetsArgs.faces[0], padding);
    if (gui.isPreviousWidgetClicked())
        m_fileBrowser.open(&m_assetsArgs.faces[0], gui);

    sl::gui::labeledTextInput(gui, "Left", m_assetsArgs.faces[1], padding);
    if (gui.isPreviousWidgetClicked())
        m_fileBrowser.open(&m_assetsArgs.faces[1], gui);

    sl::gui::labeledTextInput(gui, "Top", m_assetsArgs.faces[2], padding);
    if (gui.isPreviousWidgetClicked())
        m_fileBrowser.open(&m_assetsArgs.faces[2], gui);

    sl::gui::labeledTextInput(gui, "Bottom", m_assetsArgs.faces[3], padding);
    if (gui.isPreviousWidgetClicked())
        m_fileBrowser.open(&m_assetsArgs.faces[3], gui);

    sl::gui::labeledTextInput(gui, "Front", m_assetsArgs.faces[4], padding);
    if (gui.isPreviousWidgetClicked())
        m_fileBrowser.open(&m_assetsArgs.faces[4], gui);

    sl::gui::labeledTextInput(gui, "Back", m_assetsArgs.faces[5], padding);
    if (gui.isPreviousWidgetClicked())
        m_fileBrowser.open(&m_assetsArgs.faces[5], gui);

    m_fileBrowser.show(gui);

    if (m_loadClicked) {
        validateAssetName(m_assetsArgs.assetName);

        auto cubemap = sl::asset::AssetLoader::loadLocalPath<sl::graphics::Cubemap>(m_assetsArgs.faces);
        auto cubemapAsset = std::make_shared<sl::asset::CubemapAsset>(cubemap, m_assetsArgs.assetName);
        m_sharedState->assetManager.addAsset(cubemapAsset);
    }
}

void AssetsTab::handleModelLoader(sl::gui::GuiApi& gui) {
    sl::gui::labeledTextInput(gui, "Model", m_assetsArgs.modelName, padding);
    if (gui.isPreviousWidgetClicked())
        m_fileBrowser.open(&m_assetsArgs.modelName, gui);

    m_fileBrowser.show(gui);

    if (m_loadClicked) {
        validateAssetName(m_assetsArgs.assetName);

        auto model = sl::asset::AssetLoader::loadGlobalPath<sl::geometry::Model>(m_assetsArgs.modelName);
        auto modelAsset = std::make_shared<sl::asset::ModelAsset>(model, m_assetsArgs.assetName);
        m_sharedState->assetManager.addAsset(modelAsset);
    }
}

void AssetsTab::handleTextureLoader(sl::gui::GuiApi& gui) {
}
}
