#pragma once

#include <unordered_map>

#include <imgui/imgui.h>

#include "AssetGuiProvider.h"
#include "sl/core/GameObject.h"
#include "sl/gfx/Texture.h"
#include "sl/gui/Utils.h"

namespace sl::gui::assets {

class TextureGui {
    class Provider : public AssetGuiProvider {
    public:
        explicit Provider(gfx::Texture* texture)
            : m_texture(texture) {
        }

        void render() {
            if (m_texture.hasValue()) {
                ImGui::Text("Texture resource");
                ImGui::Text("%s", m_texture->name.c_str());
                ImGui::Separator();

                ImGui::Text("Path:");
                ImGui::Text("%s", m_texture->path.c_str());

                ImGui::Separator();

                const auto width = ImGui::GetWindowWidth();
                gui::showImage(*m_texture, { width, width });
            }
        }

    private:
        core::GameObjectPtr<gfx::Texture> m_texture;
    };

public:
    std::unique_ptr<AssetGuiProvider> createGuiProvider(gfx::Texture* texture) {
        return std::make_unique<Provider>(texture);
    }
};

}