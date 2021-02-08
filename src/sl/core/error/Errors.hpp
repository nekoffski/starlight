#pragma once

#include "BaseError.hpp"

namespace sl::core::error {

SL_DEFINE_ERROR(CoreError);
SL_DEFINE_ERROR(GraphicsError);
SL_DEFINE_ERROR(GeometryError);
SL_DEFINE_ERROR(ApplicationError);
SL_DEFINE_ERROR(AssetError);

SL_DEFINE_SUB_ERROR(ModelError, GeometryError);

SL_DEFINE_SUB_ERROR(ShaderError, GraphicsError);
SL_DEFINE_SUB_ERROR(TextureError, GraphicsError);
SL_DEFINE_SUB_ERROR(ImageError, GraphicsError);

SL_DEFINE_SUB_ERROR(WindowError, CoreError);
SL_DEFINE_SUB_ERROR(JsonError, CoreError);
}
