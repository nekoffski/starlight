#pragma once

#include <kcore/Concepts.hh>
#include <kcore/containers/FlatMap.hh>
#include <memory>

#include "Metal.hh"
#include "starlight/core/Core.hh"
#include "starlight/renderer/core/Shader.hh"

namespace sl {

class MetalShader : public kc::NonCopyable, public kc::NonMovable {
    struct Guard {};

    using Functions = kc::FlatMap<ShaderStage, MTL::Function*>;

   public:
    static Result<std::unique_ptr<MetalShader>> create(
        MetalContext& ctx, const ShaderDescription& description
    );

    MTL::Function* function(ShaderStage stage);

    explicit MetalShader(
        MTL::Library* library, Functions&& functions,
        const ShaderDescription& description, Guard
    );
    ~MetalShader();

    const ShaderDescription& description() const { return m_description; }

   private:
    MTL::Library* m_library;
    Functions m_functions;
    ShaderDescription m_description;
};

}  // namespace sl
