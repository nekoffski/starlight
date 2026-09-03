#pragma once

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"

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

}  // namespace sl