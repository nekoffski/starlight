#include "AssetsTab.h"

#include <any>

#include <ranges>

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
    if (gui.button(ICON_FA_PLUS "  Load new asset")) {
        resetArgs();
        gui.openPopUp("AssetLoadPopUp");
    }

    if (gui.beginPopUp("AssetLoadPopUp")) {
        showLoaderPopUp(gui);
        gui.endPopUp();
    }

    gui.displayText("\n");

    static std::string selectedAssetName = "";

    auto displayAssetSection = [&](const std::string& header, auto& assetsMap) {
        gui.beginGroup();

        if (gui.beginTreeNode(header)) {
            for (auto& [name, asset] : assetsMap) {
                bool isSelected = (name == selectedAssetName);

                if (isSelected)
                    gui.pushTextColor(sl::gui::selectedEntryColor);

                gui.displayText(name);

                if (isSelected)
                    gui.popColor();

                if (gui.isPreviousWidgetClicked()) {
                    selectedAssetName = name;
                    m_sharedState->activeAssetGuiProvider = m_assetsGui.createGuiProvider(asset);
                }
            }
            gui.popTreeNode();
        }
        gui.endGroup();
    };

    auto& cubemaps = m_sharedState->assetManager.getCubemaps().getAll();
    displayAssetSection("Cubemaps", cubemaps);
    gui.sameLine();

    auto& meshes = m_sharedState->assetManager.getMeshes().getAll();
    displayAssetSection("Meshes", meshes);
    gui.sameLine();

    auto shaders = m_sharedState->assetManager.getShaders().getAll();
    auto& globalShaders = GLOBALS().shaders->shadersByName;

    for (auto& [name, shader] : globalShaders)
        shaders[name] = shader;

    displayAssetSection("Shader", shaders);

    gui.endGroup();
}

void AssetsTab::resetArgs() {
    m_assetsArgs.assetName = "";
    m_assetsArgs.activeItem = 0;
    m_assetsArgs.modelName = "/tow/tower.obj";
    m_assetsArgs.faces = {
        "/skybox/right.jpg", "/skybox/left.jpg", "/skybox/top.jpg",
        "/skybox/bottom.jpg", "/skybox/front.jpg", "/skybox/back.jpg"
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
        m_fileBrowser.open(gui, [&v = m_assetsArgs.faces[0]](const std::string& value) { v = value; });

    sl::gui::labeledTextInput(gui, "Fragment shader", m_assetsArgs.faces[1], padding);
    if (gui.isPreviousWidgetClicked())
        m_fileBrowser.open(gui, [&v = m_assetsArgs.faces[1]](const std::string& value) { v = value; });

    sl::gui::labeledTextInput(gui, "Geometry shader", m_assetsArgs.faces[2], padding);
    if (gui.isPreviousWidgetClicked())
        m_fileBrowser.open(gui, [&v = m_assetsArgs.faces[2]](const std::string& value) { v = value; });

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
        m_fileBrowser.open(gui, [&v = m_assetsArgs.faces[0]](const std::string& value) { v = value; });

    sl::gui::labeledTextInput(gui, "Left", m_assetsArgs.faces[1], padding);
    if (gui.isPreviousWidgetClicked())
        m_fileBrowser.open(gui, [&v = m_assetsArgs.faces[1]](const std::string& value) { v = value; });

    sl::gui::labeledTextInput(gui, "Top", m_assetsArgs.faces[2], padding);
    if (gui.isPreviousWidgetClicked())
        m_fileBrowser.open(gui, [&v = m_assetsArgs.faces[2]](const std::string& value) { v = value; });

    sl::gui::labeledTextInput(gui, "Bottom", m_assetsArgs.faces[3], padding);
    if (gui.isPreviousWidgetClicked())
        m_fileBrowser.open(gui, [&v = m_assetsArgs.faces[3]](const std::string& value) { v = value; });

    sl::gui::labeledTextInput(gui, "Front", m_assetsArgs.faces[4], padding);
    if (gui.isPreviousWidgetClicked())
        m_fileBrowser.open(gui, [&v = m_assetsArgs.faces[4]](const std::string& value) { v = value; });

    sl::gui::labeledTextInput(gui, "Back", m_assetsArgs.faces[5], padding);
    if (gui.isPreviousWidgetClicked())
        m_fileBrowser.open(gui, [&v = m_assetsArgs.faces[5]](const std::string& value) { v = value; });

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
        m_fileBrowser.open(gui, [&v = m_assetsArgs.modelName](const std::string& value) { v = value; });

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
