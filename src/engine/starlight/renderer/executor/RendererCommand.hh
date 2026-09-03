#pragma once

#include <future>
#include <memory>
#include <variant>

#include "starlight/core/Error.hh"
#include "starlight/renderer/backend/RenderOutput.hh"
#include "starlight/renderer/rhi/RenderSurfaceProvider.hh"

namespace sl {

struct RendererNoop {};

struct RendererFlush {
    std::promise<void> completion;
};

struct RendererCreateSurfaceOutput {
    std::shared_ptr<RenderSurfaceProvider> window;
    std::promise<Result<RenderOutput>> completion;
};

using RendererCommand =
    std::variant<RendererNoop, RendererFlush, RendererCreateSurfaceOutput>;

}  // namespace sl
