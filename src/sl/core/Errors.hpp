#pragma once

#include <kc/core/ErrorBase.hpp>

namespace sl::core {

DEFINE_ERROR(CoreError);
DEFINE_ERROR(GraphicsError);
DEFINE_ERROR(GeometryError);
DEFINE_ERROR(ApplicationError);
DEFINE_ERROR(AssetError);
DEFINE_ERROR(GuiError);

DEFINE_SUB_ERROR(GuiUserError, GuiError);

DEFINE_SUB_ERROR(ModelError, GeometryError);

DEFINE_SUB_ERROR(NullValueAccessError, CoreError);

DEFINE_SUB_ERROR(ShaderError, GraphicsError);
DEFINE_SUB_ERROR(TextureError, GraphicsError);
DEFINE_SUB_ERROR(ImageError, GraphicsError);

DEFINE_SUB_ERROR(WindowError, CoreError);
DEFINE_SUB_ERROR(JsonError, CoreError);

DEFINE_SUB_ERROR(SerializationError, ApplicationError);
DEFINE_SUB_ERROR(DeserializationError, ApplicationError);

}  // namespace sl::core
