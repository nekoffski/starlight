#include "AssetsTab.h"

#include "editor/DebugConsole.hpp"
#include "sl/core/Errors.hpp"
#include "sl/core/String.hpp"
#include "sl/geom/ModelLoader.hpp"
#include "sl/gui/GuiApi.h"
#include "sl/gui/Utils.hpp"
#include "sl/utils/Globals.h"

namespace editor::gui {

constexpr int padding = 65;

static void validateAssetName(const std::string& name) {
    using namespace sl::core;

    if (sl::core::isStringEmpty(name))
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

    gui.beginGroup();
    if (gui.beginTreeNode("Cubemaps")) {
        for (auto cubemapName : m_sharedState->assetManager.getCubemaps().getNames())
            gui.displayText(cubemapName);

        gui.popTreeNode();
    }

    gui.endGroup();
    gui.sameLine();
    gui.beginGroup();

    if (gui.beginTreeNode("Meshes")) {
        for (auto meshName : m_sharedState->assetManager.getMeshes().getNames())
            gui.displayText(meshName);

        gui.popTreeNode();
    }

    gui.endGroup();
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
    } catch (sl::core::AssetError& err) {
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

        auto shader = sl::gfx::Shader::load(
            m_assetsArgs.faces[0], m_assetsArgs.faces[1], m_assetsArgs.faces[2]);
        // auto shaderAsset = std::make_shared<sl::asset::ShaderAsset>(shader, m_assetsArgs.assetName);
        // m_sharedState->assetManager.addAsset(shaderAsset);
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

        auto faces = m_assetsArgs.faces;
        for (auto& face : faces)
            face = GLOBALS().config.paths.cubemaps + face;

        m_sharedState->assetManager.add(
            sl::gfx::Cubemap::load(faces), m_assetsArgs.assetName);
    }
}

void AssetsTab::handleModelLoader(sl::gui::GuiApi& gui) {
    sl::gui::labeledTextInput(gui, "Model", m_assetsArgs.modelName, padding);
    if (gui.isPreviousWidgetClicked())
        m_fileBrowser.open(&m_assetsArgs.modelName, gui);

    m_fileBrowser.show(gui);

    if (m_loadClicked) {
        validateAssetName(m_assetsArgs.assetName);

        auto model = sl::geom::ModelLoader::load(m_assetsArgs.modelName);
        m_sharedState->assetManager.add(model->meshes);
    }
}

void AssetsTab::handleTextureLoader(sl::gui::GuiApi& gui) {
}
}
