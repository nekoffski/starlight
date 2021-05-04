#pragma once

#include "sl/gfx/Shader.h"

#include "assets/AssetGuiProvider.h"
#include "assets/ShaderGui.h"

namespace sl::gui {

class AssetsGui {
public:
    template <typename T>
    std::unique_ptr<assets::AssetGuiProvider> createGuiProvider(std::shared_ptr<T>) {
        return nullptr;
    }

    std::unique_ptr<assets::AssetGuiProvider> createGuiProvider(std::shared_ptr<gfx::Shader> shader) {
        return m_shaderGui.createGuiProvider(shader);
    }

private:
    assets::ShaderGui m_shaderGui;
};

}