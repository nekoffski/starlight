#pragma once

#include "sl/asset/AssetManager.h"
#include "sl/ecs/Component.h"

namespace sl::gui::components {

struct ComponentGui {
    virtual void renderComponentGui(
        ecs::Component& component, gui::GuiApi& gui, asset::AssetManager& assetManager) const = 0;
};

template <typename T>
class ComponentGuiImpl : public ComponentGui {
public:
    void renderComponentGui(
        ecs::Component& component, gui::GuiApi& gui, asset::AssetManager& assetManager) const override {

        renderComponentGuiImpl(
            static_cast<T&>(component), gui, assetManager);
    }

protected:
    virtual void renderComponentGuiImpl(
        T&, gui::GuiApi& gui, asset::AssetManager& assetManager) const = 0;
};

}