#pragma once

#include "BaseError.hpp"

namespace sl::core {

LOG_DEFINE_ERROR(CoreError);
LOG_DEFINE_ERROR(GraphicsError);
LOG_DEFINE_ERROR(GeometryError);
LOG_DEFINE_ERROR(ApplicationError);
LOG_DEFINE_ERROR(AssetError);
LOG_DEFINE_ERROR(GuiError);

LOG_DEFINE_SUB_ERROR(GuiUserError, GuiError);

LOG_DEFINE_SUB_ERROR(ModelError, GeometryError);

LOG_DEFINE_SUB_ERROR_DF(NullValueAccessError, CoreError, ErrorCode::NullValueAccess);

LOG_DEFINE_SUB_ERROR(ShaderError, GraphicsError);
LOG_DEFINE_SUB_ERROR(TextureError, GraphicsError);
LOG_DEFINE_SUB_ERROR(ImageError, GraphicsError);

LOG_DEFINE_SUB_ERROR(WindowError, CoreError);
LOG_DEFINE_SUB_ERROR(JsonError, CoreError);

LOG_DEFINE_SUB_ERROR(SerializationError, ApplicationError);
LOG_DEFINE_SUB_ERROR(DeserializationError, ApplicationError);
LOG_DEFINE_SUB_ERROR(ConfigError, ApplicationError);
}
