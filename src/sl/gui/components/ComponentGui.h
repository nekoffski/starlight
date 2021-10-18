#pragma once

#include <imgui_sugar.hpp>

#include "sl/asset/AssetManager.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"

namespace sl::gui::components {

struct ComponentGui {
    virtual void renderComponentGui(
        ecs::Component& component, asset::AssetManager& assetManager, ecs::Entity& entity) = 0;
};

template <typename T>
class ComponentGuiImpl : public ComponentGui {
public:
    void renderComponentGui(
        ecs::Component& component, asset::AssetManager& assetManager, ecs::Entity& entity) override {

        renderComponentGuiImpl(static_cast<T&>(component), assetManager, entity);
    }

protected:
    virtual void renderComponentGuiImpl(
        T&, asset::AssetManager& assetManager, ecs::Entity& entity) = 0;

    bool beginComponentTreeNode(const std::string& name, ecs::Component& component) const {
        ImGui::Separator();

        ImGui::SetNextItemOpen(ImGuiCond_Once, true);
        bool isOpened = ImGui::TreeNode(name.c_str());

        ImGui::SameLine();
        ImGui::SetWindowFontScale(0.6f);
        ImGui::Checkbox(("##" + name).c_str(), &component.isActive);

        ImGui::SetWindowFontScale(1.0f);

        ImGui::SameLine(ImGui::GetWindowWidth() - 35);
        ImGui::Text(ICON_FA_TIMES);

        component.shouldBeRemoved = ImGui::IsItemClicked();
        return isOpened;
    }
};

}