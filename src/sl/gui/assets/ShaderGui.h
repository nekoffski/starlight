#pragma once

#include <unordered_map>

#include "AssetGui.h"
#include "sl/gfx/Shader.h"
#include "sl/utils/Globals.h"

namespace sl::gui::assets {

class ShaderGui {
    struct Params {
        bool recompileOnSave = false;
    };

    class Handler : public AssetGui {
    public:
        explicit Handler(std::shared_ptr<gfx::Shader> shader, Params& params)
            : m_shader(shader)
            , m_params(params) {
        }

        void render(GuiApi& gui) {
            if (auto shader = m_shader.lock(); shader) {
                gui.displayText("Shader resource");
                gui.breakLine();

                if (gui.beginTreeNode("Paths:")) {
                    gui.displayText(shader->getVertexShaderPath());
                    gui.displayText(shader->getFragmentShaderPath());
                    gui.displayText(shader->getGeometryShaderPath());
                    gui.popTreeNode();
                }

                gui.breakLine();

                if (gui.beginTreeNode("Build")) {

                    if (gui.button("Recompile", gui.getCurrentWindowWidth() * 0.85f)) {
                        shader->disable();
                        gfx::ShaderCompiler::compile(*shader);
                    }

                    gui.displayText("\n");

                    if (gui.checkbox("Recompile on save", m_params.recompileOnSave)) {
                    }

                    gui.popTreeNode();
                }
            }
        }

    private:
        std::weak_ptr<gfx::Shader> m_shader;
        Params& m_params;
    };

public:
    std::unique_ptr<AssetGui> createGui(std::shared_ptr<gfx::Shader> shader) {
        return std::make_unique<Handler>(shader, m_params[shader->id]);
    }

private:
    std::unordered_map<int, Params> m_params;
};

}