#pragma once

#include "sl/gfx/Shader.h"

#include "assets/AssetGuiProvider.h"
#include "assets/ShaderGui.h"
#include "assets/TextureGui.h"

#include "sl/core/Logger.h"

namespace sl::gui {

class AssetsGui {
public:
    template <typename T>
    std::unique_ptr<assets::AssetGuiProvider> createGuiProvider(std::shared_ptr<T>) {
        SL_INFO("Creating NULL gui asset provider");
        return nullptr;
    }

    std::unique_ptr<assets::AssetGuiProvider> createGuiProvider(std::shared_ptr<gfx::Shader> shader) {
        SL_INFO("Creating shader gui asset provider");
        return m_shaderGui.createGuiProvider(shader);
    }

    // TODO: replace with raw_ptr
    std::unique_ptr<assets::AssetGuiProvider> createGuiProvider(std::unique_ptr<gfx::Texture>& texture) {
        SL_INFO("Creating texture gui asset provider");
        return m_textureGui.createGuiProvider(texture.get());
    }

private:
    assets::ShaderGui m_shaderGui;
    assets::TextureGui m_textureGui;
};

}