#include "ResourcesTab.h"

#include "sl/asset/AssetManager.hpp"
#include "sl/event/EventBus.h"
#include "sl/gui/FileBrowser.hpp"
#include "sl/gui/GuiApi.h"
#include "sl/gui/Utils.hpp"

namespace editor::gui {

constexpr int padding = 65;

ResourcesTab::ResourcesTab(res::ResourceManager& resourceManager)
    : m_resourceManager(resourceManager) {
}

void ResourcesTab::render(sl::gui::GuiApi& gui) {
    gui.beginGroup();
    if (gui.button("+"))
        gui.openPopUp("ResourceLoadPopUp");

    if (gui.beginPopUp("ResourceLoadPopUp")) {
        showLoaderPopUp(gui);
        gui.endPopUp();
    }

    gui.endGroup();

    for (auto& [_, namesVector] : m_resourceManager.getAllNames())
        for (auto& resourceName : namesVector)
            gui.displayText(resourceName);
}

void ResourcesTab::showLoaderPopUp(sl::gui::GuiApi& gui) {
    static int activeItem = 0;
    static const std::vector<std::string> labels = { "Cubemap", "Texture", "Model" };

    gui.combo(sl::gui::createHiddenLabel("ResourcesCombo"), activeItem, labels);

    m_loadClicked = false;
    if (gui.button("Load")) {
        gui.closeCurrentPopUp();
        m_loadClicked = true;
    }

    gui.sameLine(250);
    gui.beginGroup();

    gui.displayText("Name");
    gui.sameLine(padding);
    gui.inputText(sl::gui::createHiddenLabel("Name"), m_resourceName);

    switch (activeItem) {
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

    gui.endGroup();
}

void ResourcesTab::handleCubemapLoader(sl::gui::GuiApi& gui) {
    static std::array<std::string, 6> faces = {
        "/skybox/top.jpg", "/skybox/bottom.jpg", "/skybox/right.jpg", "/skybox/left.jpg", "/skybox/front.jpg", "/skybox/back.jpg"
    };

    static sl::gui::FileBrowser fileBrowser{ gui, "cubemapLoaderPopUp" };

    sl::gui::labeledTextInput(gui, "Top", faces[0], padding);
    if (gui.isPreviousWidgetClicked())
        fileBrowser.open(&faces[0]);

    sl::gui::labeledTextInput(gui, "Bottom", faces[1], padding);
    if (gui.isPreviousWidgetClicked())
        fileBrowser.open(&faces[1]);

    sl::gui::labeledTextInput(gui, "Right", faces[2], padding);
    if (gui.isPreviousWidgetClicked())
        fileBrowser.open(&faces[2]);

    sl::gui::labeledTextInput(gui, "Left", faces[3], padding);
    if (gui.isPreviousWidgetClicked())
        fileBrowser.open(&faces[3]);

    sl::gui::labeledTextInput(gui, "Front", faces[4], padding);
    if (gui.isPreviousWidgetClicked())
        fileBrowser.open(&faces[4]);

    sl::gui::labeledTextInput(gui, "Back", faces[5], padding);
    if (gui.isPreviousWidgetClicked())
        fileBrowser.open(&faces[5]);

    fileBrowser.show();

    if (m_loadClicked) {
        auto cubemap = sl::asset::AssetManager::loadLocalPath<sl::graphics::Cubemap>(
            faces[0], faces[1], faces[2], faces[3], faces[4], faces[5]);

        auto cubemapResource = std::make_shared<res::CubemapResource>(cubemap, m_resourceName);
        m_resourceManager.addResource(cubemapResource);
    }
}

void ResourcesTab::handleModelLoader(sl::gui::GuiApi& gui) {
    static std::string modelName = "/tow/tower.obj";
    static sl::gui::FileBrowser fileBrowser{ gui, "modelLoaderPopUp" };

    sl::gui::labeledTextInput(gui, "Model", modelName, padding);
    if (gui.isPreviousWidgetClicked())
        fileBrowser.open(&modelName);

    fileBrowser.show();

    if (m_loadClicked) {
        auto model = sl::asset::AssetManager::loadGlobalPath<sl::geometry::Model>(modelName);
        auto modelResource = std::make_shared<res::ModelResource>(model, m_resourceName);
        m_resourceManager.addResource(modelResource);
    }
}

void ResourcesTab::handleTextureLoader(sl::gui::GuiApi& gui) {
}
}
