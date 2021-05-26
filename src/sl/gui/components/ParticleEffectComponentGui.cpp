#include "ParticleEffectComponentGui.h"

#include "sl/core/Logger.h"

#include <ranges>

namespace sl::gui::components {

using namespace scene::components;

void ParticleEffectComponentGui::renderComponentGuiImpl(ParticleEffectComponent& component,
    gui::GuiApi& gui, asset::AssetManager& assetManager, ecs::Entity& entity) {

    auto& params = m_params[component.ownerEntityId];

    gui.pushId(component.ownerEntityId);

    if (beginComponentTreeNode(gui, "Particle effect", component)) {
        gui.displayText("Position");
        gui.dragFloat3(gui::createHiddenLabel("pfxPositon"), component.position, 0.1f);

        auto textures = assetManager.getTextures();

        std::vector<std::string> names = { "None" };
        std::ranges::move(textures.getNames(), std::back_inserter(names));

        gui.displayText("Texture:");

        if (gui.combo("##pfx_texture", params.selectedTexture, names)) {
            if (params.selectedTexture == 0) {
                component.texture = nullptr;
                SL_INFO("Setting null texture for component: {}", entity.asString());
            } else {
                auto& name = names[params.selectedTexture];
                SL_INFO("Setting {} texture for component: {}", name, entity.asString());
                component.texture = textures.getByName(name);
            }
        }

        gui.displayText("Max particles");
        gui.dragInt(gui::createHiddenLabel("pfxMaxParticles"), component.maxParticles, 1.0, 1, 12500);

        auto& generatorSettings = component.pfxGeneratorSettings;

        gui.displayText("Radius");
        gui.dragFloat(gui::createHiddenLabel("pfxRadius"), generatorSettings.radius, 0.01f, 0.0f, 25.0f);

        gui.displayText("Min color");
        gui.dragFloat3(gui::createHiddenLabel("pfxMinColor"), generatorSettings.minColor, 0.01f, 0.0f, 1.0f);

        gui.displayText("Max color");
        gui.dragFloat3(gui::createHiddenLabel("pfxMaxColor"), generatorSettings.maxColor, 0.01f, 0.0f, 1.0f);

        gui.displayText("Min speed");
        gui.dragFloat(gui::createHiddenLabel("pfxMinSpeed"), generatorSettings.minSpeed, 0.005f, 0.0f, 15.0f);

        gui.displayText("Max speed");
        gui.dragFloat(gui::createHiddenLabel("pfxMaxSpeed"), generatorSettings.maxSpeed, 0.005f, 0.0f, 15.0f);

        gui.displayText("Min scale");
        gui.dragFloat(gui::createHiddenLabel("pfxMinScale"), generatorSettings.minScale, 0.005f, 0.0f, 1.0f);

        gui.displayText("Max scale");
        gui.dragFloat(gui::createHiddenLabel("pfxMaxScale"), generatorSettings.maxScale, 0.005f, 0.0f, 1.0f);

        gui.displayText("Delta scale");
        gui.dragFloat(gui::createHiddenLabel("pfxDeltaScale"), generatorSettings.deltaScale, 0.001f, 0.0f, 1.0f);

        gui.displayText("Direction factor");
        gui.dragFloat(gui::createHiddenLabel("pfxDirectionFactor"), generatorSettings.directionFactor, 0.01f, -5.0f, 5.0f);

        gui.popTreeNode();
    }

    gui.popId();
}
}