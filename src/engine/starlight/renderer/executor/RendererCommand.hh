#pragma once

#include <future>
#include <memory>
#include <variant>

#include "starlight/core/Error.hh"
#include "starlight/renderer/core/Primitive.hh"
#include "starlight/renderer/core/Shader.hh"
#include "starlight/renderer/frontend/RenderRequest.hh"
#include "starlight/renderer/rhi/RenderSurfaceProvider.hh"

namespace sl {

struct RendererNoop {};

struct RendererFlush {
    std::promise<void> completion;
};

struct RendererCreateSurfaceTarget {
    std::shared_ptr<RenderSurfaceProvider> surfaceProvider;
    std::promise<Result<RenderTarget>> completion;
};

struct RendererSubmit {
    std::promise<Result<void>> completion;
    RenderRequest request;
};

struct RendererCreateShader {
    ShaderDescription description;
    std::promise<Result<ShaderHandle>> completion;
};

struct RendererDestroyShader {
    ShaderHandle handle;
};

struct RendererUploadPrimitive {
    PrimitiveUploadData description;
    std::promise<Result<Primitive>> completion;
};

struct RendererFreePrimitive {
    Primitive primitive;
};

using RendererCommand = std::variant<
    RendererNoop, RendererFlush, RendererCreateSurfaceTarget, RendererSubmit,
    RendererCreateShader, RendererDestroyShader, RendererUploadPrimitive,
    RendererFreePrimitive>;

}  // namespace sl
