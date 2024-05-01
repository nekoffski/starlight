#include "Vertex.h"

#include "starlight/core/Log.h"

namespace sl {

std::string Vertex3::toString() const {
    return fmt::format(
      "Vertex3: position={}, texture coordinates={}, normal={}, color={}, tangent={}",
      position, textureCoordinates, normal, color, tangent
    );
}

}  // namespace sl