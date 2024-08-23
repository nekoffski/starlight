#include "Vertex.hh"

#include "starlight/core/Log.hh"

namespace sl {

std::string Vertex3::toString() const {
    return fmt::format(
      "Vertex3: position={}, texture coordinates={}, normal={}, color={}, tangent={}",
      position, textureCoordinates, normal, color, tangent
    );
}

}  // namespace sl