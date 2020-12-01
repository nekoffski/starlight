#pragma once

#include "Widget.h"

#include "editor/res/ResourceManager.h"
#include "sl/core/event/EventBus.h"
#include "sl/gui/GUIProxy.h"
#include "sl/gui/Utils.hpp"
#include "sl/asset/AssetManager.hpp"

#include "editor/res/ResourceManager.h"

namespace editor::gui {

using namespace sl::core;

class ResourcesTab : public Widget {
public:
    explicit ResourcesTab(res::ResourceManager& resourceManager)
        : m_resourceManager(resourceManager) {
    }

    void render(sl::gui::GUIProxy& gui) override {

        gui->beginGroup();
        if (gui->button("+"))
            gui->openPopUp("ResourceLoadPopUp");

        if (gui->beginPopUp("ResourceLoadPopUp")) {
            static int activeItem = 0;
            std::vector<std::string> labels = { "Cubemap", "Texture", "Model" };
            gui->combo(sl::gui::createHiddenLabel("ResourcesCombo"), activeItem, labels);

            bool clicked = false;
            if (gui->button("Load")) {
                gui->closeCurrentPopUp();
                clicked = true;
            }

            gui->sameLine(250);
            gui->beginGroup();

            constexpr int padding = 65;
            gui->displayText("Name");
            gui->sameLine(padding);
            gui->inputText(sl::gui::createHiddenLabel("Name"), name);

            switch (activeItem) {
            case 0: {
                gui->displayText("Top");
                gui->sameLine(padding);
                gui->inputText(sl::gui::createHiddenLabel("Top"), top);

                gui->displayText("Bottom");
                gui->sameLine(padding);
                gui->inputText(sl::gui::createHiddenLabel("Bottom"), bottom);

                gui->displayText("Right");
                gui->sameLine(padding);
                gui->inputText(sl::gui::createHiddenLabel("Right"), right);

                gui->displayText("Left");
                gui->sameLine(padding);
                gui->inputText(sl::gui::createHiddenLabel("Left"), left);

                gui->displayText("Front");
                gui->sameLine(padding);
                gui->inputText(sl::gui::createHiddenLabel("Front"), front);

                gui->displayText("Back");
                gui->sameLine(padding);
                gui->inputText(sl::gui::createHiddenLabel("Back"), back);

                if (clicked) {
                    auto cubemap = sl::asset::AssetManager::load<sl::platform::texture::Cubemap>(
                        right, left, top, bottom, front, back);
                    auto cubemapResource = std::make_shared<res::CubemapResource>(cubemap, name);
                    m_resourceManager.addResource(cubemapResource);
                }
            }
            }
            gui->endGroup();

            gui->endPopUp();
        }

        gui->endGroup();

        for (auto& [_, namesVector] : m_resourceManager.getAllNames())
            for (auto& resourceName : namesVector)
                gui->displayText(resourceName);
    }

private:
    res::ResourceManager& m_resourceManager;

    std::string name;
    std::string top = "/skybox/top.jpg";
    std::string bottom = "/skybox/bottom.jpg";
    std::string left = "/skybox/left.jpg";
    std::string right = "/skybox/right.jpg";
    std::string front = "/skybox/front.jpg";
    std::string back = "/skybox/back.jpg";
};
}
