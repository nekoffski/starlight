#include "ResourcesTab.h"

#include "editor/DebugConsole.hpp"
#include "sl/asset/AssetManager.hpp"
#include "sl/core/error/Errors.hpp"
#include "sl/core/utils/String.hpp"
#include "sl/gui/FileBrowser.hpp"
#include "sl/gui/GuiApi.h"
#include "sl/gui/Utils.hpp"

namespace editor::gui {

constexpr int padding = 65;

static void validateResourceName(const std::string& name) {
    using namespace sl::core::error;

    SL_INFO("?? {}", name.size());
    if (sl::core::utils::isStringEmpty(name))
        throw AssetError(ErrorCode::AssetError, "Asset name cannot be empty");
}

ResourcesTab::ResourcesTab(res::ResourceManager& resourceManager)
    : m_resourceManager(resourceManager) {
}

void ResourcesTab::render(sl::gui::GuiApi& gui) {
    gui.beginGroup();
    if (gui.button("+")) {
        resetArgs();
        gui.openPopUp("ResourceLoadPopUp");
    }

    if (gui.beginPopUp("ResourceLoadPopUp")) {
        showLoaderPopUp(gui);
        gui.endPopUp();
    }

    gui.endGroup();

    for (auto& [_, namesVector] : m_resourceManager.getAllNames())
        for (auto& resourceName : namesVector)
            gui.displayText(resourceName);
}

void ResourcesTab::resetArgs() {
    m_resourcesArgs.resourceName = "";
    m_resourcesArgs.activeItem = 0;
    m_resourcesArgs.modelName = "/tow/tower.obj";
    m_resourcesArgs.faces = {
        "/skybox/top.jpg", "/skybox/bottom.jpg", "/skybox/right.jpg", "/skybox/left.jpg", "/skybox/front.jpg", "/skybox/back.jpg"
    };
}

void ResourcesTab::showLoaderPopUp(sl::gui::GuiApi& gui) {
    static const std::vector<std::string> labels = { "Cubemap", "Texture", "Model" };

    gui.combo(sl::gui::createHiddenLabel("ResourcesCombo"), m_resourcesArgs.activeItem, labels);

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
    gui.inputText(sl::gui::createHiddenLabel("Name"), m_resourcesArgs.resourceName);

    try {
        switch (m_resourcesArgs.activeItem) {
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
        }
    } catch (sl::core::error::AssetError& err) {
        m_errorDialog.setErrorMessage(err.getDetails(), gui);
        WRITE_DEBUG(err.as<std::string>());
        m_loadClicked = false;
    }

    if (m_loadClicked) {
        gui.closeCurrentPopUp();
    }

    gui.endGroup();

    m_errorDialog.show(gui);
}

void ResourcesTab::handleCubemapLoader(sl::gui::GuiApi& gui) {
    sl::gui::FileBrowser fileBrowser{ gui, "cubemapLoaderPopUp" };

    sl::gui::labeledTextInput(gui, "Top", m_resourcesArgs.faces[0], padding);
    if (gui.isPreviousWidgetClicked())
        fileBrowser.open(&m_resourcesArgs.faces[0]);

    sl::gui::labeledTextInput(gui, "Bottom", m_resourcesArgs.faces[1], padding);
    if (gui.isPreviousWidgetClicked())
        fileBrowser.open(&m_resourcesArgs.faces[1]);

    sl::gui::labeledTextInput(gui, "Right", m_resourcesArgs.faces[2], padding);
    if (gui.isPreviousWidgetClicked())
        fileBrowser.open(&m_resourcesArgs.faces[2]);

    sl::gui::labeledTextInput(gui, "Left", m_resourcesArgs.faces[3], padding);
    if (gui.isPreviousWidgetClicked())
        fileBrowser.open(&m_resourcesArgs.faces[3]);

    sl::gui::labeledTextInput(gui, "Front", m_resourcesArgs.faces[4], padding);
    if (gui.isPreviousWidgetClicked())
        fileBrowser.open(&m_resourcesArgs.faces[4]);

    sl::gui::labeledTextInput(gui, "Back", m_resourcesArgs.faces[5], padding);
    if (gui.isPreviousWidgetClicked())
        fileBrowser.open(&m_resourcesArgs.faces[5]);

    fileBrowser.show();

    if (m_loadClicked) {
        validateResourceName(m_resourcesArgs.resourceName);

        auto cubemap = sl::asset::AssetManager::loadLocalPath<sl::graphics::Cubemap>(m_resourcesArgs.faces);
        auto cubemapResource = std::make_shared<res::CubemapResource>(cubemap, m_resourcesArgs.resourceName);
        m_resourceManager.addResource(cubemapResource);
    }
}

void ResourcesTab::handleModelLoader(sl::gui::GuiApi& gui) {
    sl::gui::FileBrowser fileBrowser{ gui, "modelLoaderPopUp" };

    sl::gui::labeledTextInput(gui, "Model", m_resourcesArgs.modelName, padding);
    if (gui.isPreviousWidgetClicked())
        fileBrowser.open(&m_resourcesArgs.modelName);

    fileBrowser.show();

    if (m_loadClicked) {
        validateResourceName(m_resourcesArgs.resourceName);

        auto model = sl::asset::AssetManager::loadGlobalPath<sl::geometry::Model>(m_resourcesArgs.modelName);
        auto modelResource = std::make_shared<res::ModelResource>(model, m_resourcesArgs.resourceName);
        m_resourceManager.addResource(modelResource);
    }
}

void ResourcesTab::handleTextureLoader(sl::gui::GuiApi& gui) {
}
}
