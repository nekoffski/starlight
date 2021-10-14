#include "ParticleEffectComponentGui.h"

#include <kc/core/Log.h>

#include <ranges>

namespace sl::gui::components {

using namespace scene::components;

void ParticleEffectComponentGui::renderComponentGuiImpl(ParticleEffectComponent& component,
    asset::AssetManager& assetManager, ecs::Entity& entity) {

    // auto& params = m_params[component.ownerEntityId];

    // gui.pushId(component.ownerEntityId);

    // if (beginComponentTreeNode(gui, "Particle effect", component)) {
    //     ImGui::Text("Position");
    //     ImGui::DragFloat3(gui::createHiddenLabel("pfxPositon"), component.position, 0.1f);

    //     auto& textures = assetManager.getTextures();

    //     std::vector<std::string> names = { "None" };
    //     std::ranges::move(textures.getNames(), std::back_inserter(names));

    //     ImGui::Text("Texture:");

    //     if (gui.combo("##pfx_texture", params.selectedTexture, names)) {
    //         if (params.selectedTexture == 0) {
    //             component.texture.reset();
    //             LOG_INFO("Setting null texture for component: {}", entity.asString());
    //         } else {
    //             auto& name = names[params.selectedTexture];
    //             LOG_INFO("Setting {} texture for component: {}", name, entity.asString());
    //             component.texture = textures.getByName(name);
    //         }
    //     }

    //     ImGui::Text("Max particles");
    //     gui.dragInt(gui::createHiddenLabel("pfxMaxParticles"), component.maxParticles, 1.0, 1, 12500);

    //     auto& generatorSettings = component.pfxGeneratorSettings;

    //     ImGui::Text("Radius");
    //     ImGui::DragFloat(gui::createHiddenLabel("pfxRadius"), generatorSettings.radius, 0.01f, 0.0f, 25.0f);

    //     ImGui::Text("Min color");
    //     ImGui::DragFloat3(gui::createHiddenLabel("pfxMinColor"), generatorSettings.minColor, 0.01f, 0.0f, 1.0f);

    //     ImGui::Text("Max color");
    //     ImGui::DragFloat3(gui::createHiddenLabel("pfxMaxColor"), generatorSettings.maxColor, 0.01f, 0.0f, 1.0f);

    //     ImGui::Text("Min speed");
    //     ImGui::DragFloat(gui::createHiddenLabel("pfxMinSpeed"), generatorSettings.minSpeed, 0.005f, 0.0f, 15.0f);

    //     ImGui::Text("Max speed");
    //     ImGui::DragFloat(gui::createHiddenLabel("pfxMaxSpeed"), generatorSettings.maxSpeed, 0.005f, 0.0f, 15.0f);

    //     ImGui::Text("Min scale");
    //     ImGui::DragFloat(gui::createHiddenLabel("pfxMinScale"), generatorSettings.minScale, 0.005f, 0.0f, 1.0f);

    //     ImGui::Text("Max scale");
    //     ImGui::DragFloat(gui::createHiddenLabel("pfxMaxScale"), generatorSettings.maxScale, 0.005f, 0.0f, 1.0f);

    //     ImGui::Text("Delta scale");
    //     ImGui::DragFloat(gui::createHiddenLabel("pfxDeltaScale"), generatorSettings.deltaScale, 0.001f, 0.0f, 1.0f);

    //     ImGui::Text("Direction factor");
    //     ImGui::DragFloat(gui::createHiddenLabel("pfxDirectionFactor"), generatorSettings.directionFactor, 0.01f, -5.0f, 5.0f);

    //     gui.popTreeNode();
    // }

    // gui.popId();
}
}