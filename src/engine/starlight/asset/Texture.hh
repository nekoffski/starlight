#pragma once

#include "starlight/core/Error.hh"
#include "starlight/renderer/core/Texture.hh"

namespace sl {

Result<TextureUploadData> makeCheckerboard(u32 size, u32 cellSize);

}  // namespace sl
