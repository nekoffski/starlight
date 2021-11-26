#pragma once

#include <unordered_map>

#include "AssetGuiProvider.h"
#include "sl/async/PeriodicTask.h"
#include "sl/gfx/Shader.h"
#include "sl/glob/Globals.h"

namespace sl::gui::assets {

class ShaderGui {
    struct Params {
        bool recompileOnSave = false;
        std::optional<async::PeriodicTask::Handle> taskHandle = {};
    };

    class Provider : public AssetGuiProvider {
    public:
        explicit Provider(std::shared_ptr<gfx::Shader> shader, Params& params);

        void render();

    private:
        void processRecompileOnSaveRequest(std::shared_ptr<gfx::Shader>& shader);

        std::weak_ptr<gfx::Shader> m_shader;
        Params& m_params;
    };

public:
    std::unique_ptr<AssetGuiProvider> createGuiProvider(std::shared_ptr<gfx::Shader> shader);

private:
    std::unordered_map<std::string, Params> m_params;
};

}