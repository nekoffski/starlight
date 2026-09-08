#pragma once

#include <variant>

#include "starlight/core/Core.hh"
#include "starlight/core/Handle.hh"

namespace sl {

enum class ResourceType { surface, texture };

template <ResourceType T>
struct RenderResourceHandle : HandleBase<ResourceType, T> {
    using HandleBase<ResourceType, T>::HandleBase;
};

using SurfaceHandle = RenderResourceHandle<ResourceType::surface>;
using TextureHandle = RenderResourceHandle<ResourceType::texture>;

using RenderTarget = std::variant<SurfaceHandle, TextureHandle>;

}  // namespace sl

template <sl::ResourceType T>
struct std::hash<sl::RenderResourceHandle<T>> {
    size_t operator()(const sl::RenderResourceHandle<T>& h) const noexcept {
        const auto key = static_cast<sl::u64>(h.id) << 32u | h.generation;
        return std::hash<sl::u64>{}(key);
    }
};