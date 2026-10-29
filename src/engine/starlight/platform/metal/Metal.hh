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

// Reserve the last Metal buffer slot for geometry, away from Slang's low slots.
constexpr u32 metalVertexBufferIndex = 30;
constexpr u32 metalMaxInlineBufferSize = 4096;

Str parseMetalError(NS::Error* error);
MTL::VertexFormat toMetal(const ValueType& type);

}  // namespace sl
