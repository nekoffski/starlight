#pragma once

#include <unordered_map>

#include "AssetGuiProvider.h"
#include "sl/core/FileSystem.h"
#include "sl/gfx/Texture.h"

namespace sl::gui::assets {

class TextureGui {
    class Provider : public AssetGuiProvider {
    public:
        explicit Provider(std::shared_ptr<gfx::Texture> texture)
            : m_texture(texture) {
        }

        void render(GuiApi& gui) {
            if (auto texture = m_texture.lock(); texture) {
                gui.displayText("Texture resource");
                gui.displayText(texture->name);
                gui.breakLine();

                gui.displayText("Path:");
                gui.displayText(texture->path);

                gui.breakLine();

                auto width = gui.getCurrentWindowWidth();
                gui.showImage(*texture, { width, width });
            }
        }

    private:
        std::weak_ptr<gfx::Texture> m_texture;
    };

public:
    std::unique_ptr<AssetGuiProvider> createGuiProvider(std::shared_ptr<gfx::Texture> texture) {
        return std::make_unique<Provider>(texture);
    }
};

}