#include "MeshRendererComponent.h"

#include "sl/utils/Globals.h"

namespace sl::scene::components {

MeshRendererComponent::MeshRendererComponent()
    : shader(GLOBALS().shaders->defaultModelShader)
    , polygonMode(STARL_FILL) {
}

void MeshRendererComponent::onGui(gui::GuiApi& gui, asset::AssetManager& assetManager) {
    gui.pushId(ownerEntityId);

    if (beginComponentTreeNode(gui, ICON_FA_PENCIL_ALT "  Mesh renderer")) {
        static int polygonModeItem = 0;

        auto getPolygonMode = [&]() {
            switch (polygonModeItem) {
            case 0:
                return STARL_FILL;
            case 1:
                return STARL_LINE;
            case 2:
                return STARL_POINT;
            default:
                return STARL_FILL;
            }
        };

        std::vector<std::string> polygonModeLabels = {
            "Fill", "Lines", "Points"
        };

        gui.displayText("Polygon rendering mode");
        gui.combo("##polygonRenderingMode", polygonModeItem, polygonModeLabels);

        polygonMode = getPolygonMode();

        gui.popTreeNode();
    }

    gui.popId();
}

void MeshRendererComponent::serialize(core::JsonBuilder& builder) {
    builder.addField("name", "MeshRendererComponent"s).addField("shader-id", shader->id);
}

void MeshRendererComponent::deserialize(std::shared_ptr<ecs::Entity> entity, asset::AssetManager& assetManager, Json::Value& componentDescription) {
    auto& component = entity->addComponent<MeshRendererComponent>();

    auto& defaultShader = GLOBALS().shaders->defaultModelShader;

    if (defaultShader->id == componentDescription["shader-id"].asInt())
        component.shader = defaultShader;
}
}
