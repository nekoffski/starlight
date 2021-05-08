#pragma once

#include "sl/asset/AssetManager.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"

namespace sl::gui::components {

struct ComponentGui {
    virtual void renderComponentGui(
        ecs::Component& component, gui::GuiApi& gui, asset::AssetManager& assetManager, ecs::Entity& entity) = 0;
};

template <typename T>
class ComponentGuiImpl : public ComponentGui {
public:
    void renderComponentGui(
        ecs::Component& component, gui::GuiApi& gui, asset::AssetManager& assetManager, ecs::Entity& entity) override {

        renderComponentGuiImpl(static_cast<T&>(component), gui, assetManager, entity);
    }

protected:
    virtual void renderComponentGuiImpl(
        T&, gui::GuiApi& gui, asset::AssetManager& assetManager, ecs::Entity& entity) = 0;

    bool beginComponentTreeNode(gui::GuiApi& gui, const std::string& name, ecs::Component& component) const {
        gui.separator();

        bool isOpened = gui.beginTreeNode(name);

        gui.sameLine();
        gui.setFontScale(0.6f);
        gui.checkbox("##" + name, component.isActive);

        gui.setFontScale(1.0f);

        gui.sameLine(gui.getCurrentWindowWidth() - 35);
        gui.displayText(ICON_FA_TIMES);

        component.shouldBeRemoved = gui.isPreviousWidgetClicked();
        return isOpened;
    }
};

}