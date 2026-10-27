#pragma once

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"
#include "starlight/renderer/core/ShaderDataLayout.hh"

namespace sl {

class MetalContext : public NonCopyable, public NonMovable {
   public:
    explicit MetalContext();
    ~MetalContext();

    MTL::Device& device();
    MTL::CommandQueue& commandQueue();

   private:
    MTL::Device* m_device{nullptr};
    MTL::CommandQueue* m_queue{nullptr};
};

Str parseMetalError(NS::Error* error);
MTL::VertexFormat toMetal(const ValueType& type);

}  // namespace sl
