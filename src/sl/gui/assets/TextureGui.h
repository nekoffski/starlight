#pragma once

#include <unordered_map>

#include "AssetGuiProvider.h"
#include "sl/core/GameObject.h"
#include "sl/gfx/Texture.h"

namespace sl::gui::assets {

class TextureGui {
    class Provider : public AssetGuiProvider {
    public:
        explicit Provider(gfx::Texture* texture)
            : m_texture(texture) {
        }

        void render(GuiApi& gui) {
            if (m_texture.hasValue()) {
                gui.displayText("Texture resource");
                gui.displayText(m_texture->name);
                gui.breakLine();

                gui.displayText("Path:");
                gui.displayText(m_texture->path);

                gui.breakLine();

                auto width = gui.getCurrentWindowWidth();
                gui.showImage(*m_texture, { width, width });
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