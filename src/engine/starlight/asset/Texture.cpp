#include "Texture.hh"

namespace sl {

Result<TextureUploadData> makeCheckerboard(u32 size, u32 cellSize) {
    if (size == 0 || size > maxTextureDimension2D || cellSize == 0) {
        return Error::unexpected(
            ErrorCode::invalidArgument, "Invalid checkerboard dimensions"
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
