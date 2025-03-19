#pragma once

#include <functional>
#include <optional>
#include <unordered_map>

#include <starlight/app/scene/Scene.hh>
#include <starlight/app/scene/Entity.hh>
#include <starlight/renderer/RenderGraph.hh>
#include <starlight/ui/widgets/Image.hh>
#include <starlight/renderer/gpu/Texture.hh>
#include <starlight/renderer/camera/Camera.hh>

namespace sle {

struct Config {
    static Config createDefault();

    sl::f32 panelWidthRatio;
    sl::f32 panelHeightRatio;
};

struct Data {
    using Callback = std::function<void()>;

    sl::ui::ImageHandle *getTextureImage(sl::Texture &texture);

    Config config;

    sl::Scene *scene                          = nullptr;
    sl::RenderGraph *renderGraph              = nullptr;
    sl::Entity *selectedEntity                = nullptr;
    sl::Camera *camera                        = nullptr;
    std::optional<Callback> inspectorCallback = {};

    std::unordered_map<sl::u64, sl::UniquePtr<sl::ui::ImageHandle>> images = {};
};

}  // namespace sle
