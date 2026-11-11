#include "Texture.hh"

namespace sl {

TextureAsset::TextureAsset(RendererProxy proxy) : m_proxy(std::move(proxy)) {}

Result<void> TextureAsset::replace(const TextureUploadData& data) {
    checkThread();
    if (pending()) {
        return Error::unexpected(
            ErrorCode::assetBusy, "Texture creation or replacement is pending"
        );
    }
    auto future = m_proxy.createTextureAsync(data);
    if (not future) {
        return fail(future.error());
    }
    adoptCreationFuture(std::move(*future));
    return {};
}

Result<void> TextureRef::replace(const TextureUploadData& data) {
    if (not m_record) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Empty texture reference"
        );
    }
    return m_record->replace(data);
}

Result<TextureUploadData> makeCheckerboard(u32 size, u32 cellSize) {
    if (size == 0 || size > maxTextureDimension2D || cellSize == 0) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Invalid checkerboard dimensions"
        );
    }
    TextureUploadData data{.width = size, .height = size};
    data.pixels.resize(u64(size) * size * rgba8BytesPerPixel);
    for (u32 y = 0; y < size; ++y) {
        for (u32 x = 0; x < size; ++x) {
            const auto color =
                Byte(((x / cellSize + y / cellSize) % 2) ? 32 : 240);
            const auto offset = (u64(y) * size + x) * rgba8BytesPerPixel;
            data.pixels[offset] = color;
            data.pixels[offset + 1] = color;
            data.pixels[offset + 2] = color;
            data.pixels[offset + 3] = Byte(255);
        }
    }
    return data;
}

}  // namespace sl
