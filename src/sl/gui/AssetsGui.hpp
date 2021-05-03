#pragma once

#include "sl/gfx/Shader.h"

#include "assets/AssetGui.h"
#include "assets/ShaderGui.h"

namespace sl::gui {

class AssetsGui {
public:
    template <typename T>
    std::unique_ptr<assets::AssetGui> createGui(std::shared_ptr<T>) {
        return nullptr;
    }

    std::unique_ptr<assets::AssetGui> createGui(std::shared_ptr<gfx::Shader> shader) {
        return m_shaderGui.createGui(shader);
    }

private:
    assets::ShaderGui m_shaderGui;
};

}