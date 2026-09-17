#pragma once

#include <memory>

#include "Metal.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/FlatMap.hh"
#include "starlight/renderer/rhi/Shader.hh"

namespace sl {

class MetalShader : public NonCopyable, public NonMovable {
    struct Guard {};

    using Functions = FlatMap<ShaderStage, MTL::Function*>;

   public:
    static Result<std::unique_ptr<MetalShader>> create(
        MetalContext& ctx, const ShaderDescription& description
    );

    MTL::Function* function(ShaderStage stage);

    explicit MetalShader(MTL::Library* library, Functions&& functions, Guard);
    ~MetalShader();

   private:
    MTL::Library* m_library;
    Functions m_functions;
};

}  // namespace sl
