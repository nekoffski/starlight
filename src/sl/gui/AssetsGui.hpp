#pragma once

#include "sl/gfx/Shader.h"

#include "assets/AssetGuiProvider.h"
#include "assets/ShaderGui.h"
#include "assets/TextureGui.h"

#include <kc/core/Log.h>

namespace sl::gui {

class AssetsGui {
public:
    template <typename T>
    std::unique_ptr<assets::AssetGuiProvider> createGuiProvider(std::shared_ptr<T>&) {
        LOG_INFO("Creating NULL gui asset provider");
        return nullptr;
    }

    template <typename T>
    std::unique_ptr<assets::AssetGuiProvider> createGuiProvider(std::unique_ptr<T>&) {
        LOG_INFO("Creating NULL gui asset provider");
        return nullptr;
    }

    std::unique_ptr<assets::AssetGuiProvider> createGuiProvider(std::shared_ptr<gfx::Shader> shader) {
        LOG_INFO("Creating shader gui asset provider");
        return m_shaderGui.createGuiProvider(shader);
    }

    // TODO: replace with raw_ptr
    std::unique_ptr<assets::AssetGuiProvider> createGuiProvider(std::unique_ptr<gfx::Texture>& texture) {
        LOG_INFO("Creating texture gui asset provider");
        return m_textureGui.createGuiProvider(texture.get());
    }

private:
    assets::ShaderGui m_shaderGui;
    assets::TextureGui m_textureGui;
};

}