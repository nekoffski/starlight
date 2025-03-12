#pragma once

#include <functional>
#include <optional>

#include <starlight/app/scene/Scene.hh>
#include <starlight/app/scene/Entity.hh>
#include <starlight/renderer/RenderGraph.hh>

namespace sle {

struct Data {
    using Callback = std::function<void()>;

    sl::Scene *scene                          = nullptr;
    sl::RenderGraph *renderGraph              = nullptr;
    sl::Entity *selectedEntity                = nullptr;
    std::optional<Callback> inspectorCallback = {};
};

}  // namespace sle
