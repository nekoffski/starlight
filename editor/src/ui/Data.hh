#pragma once

#include <functional>
#include <optional>
#include <unordered_map>

#include <starlight/app/scene/Scene.hh>
#include <starlight/app/scene/Entity.hh>
#include <starlight/renderer/RenderGraph.hh>
#include <starlight/ui/widgets/Image.hh>
#include <starlight/renderer/gpu/Texture.hh>

namespace sle {

struct Data {
    using Callback = std::function<void()>;

    sl::ui::ImageHandle *getTextureImage(sl::Texture &texture);

    sl::Scene *scene                          = nullptr;
    sl::RenderGraph *renderGraph              = nullptr;
    sl::Entity *selectedEntity                = nullptr;
    std::optional<Callback> inspectorCallback = {};

    std::unordered_map<sl::u64, sl::UniquePtr<sl::ui::ImageHandle>> images = {};
};

}  // namespace sle
