#pragma once

#include "RenderResource.hh"

namespace sl {

enum class TextureFormat { bgra8unorm, rgba8unorm };

// Texture/sampler handle IDs are stable shader table indices until destruction.
constexpr u32 resourceTableCapacity = 64;
constexpr u32 maxTextureDimension2D = 16384;
constexpr u32 rgba8BytesPerPixel = 4;

struct TextureUploadData {
    u32 width{0};
    u32 height{0};
    TextureFormat format{TextureFormat::rgba8unorm};
    Bytes pixels;
};

enum class TextureFilter { nearest, linear };
enum class TextureAddressMode { repeat, clampToEdge };

struct SamplerDescription {
    TextureFilter filter{TextureFilter::nearest};
    TextureAddressMode addressMode{TextureAddressMode::repeat};
};

struct ShaderResourceDependencies {
    std::vector<TextureHandle> textures;
    std::vector<SamplerHandle> samplers;
};

}  // namespace sl
