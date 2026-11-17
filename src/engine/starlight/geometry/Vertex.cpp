#include "Vertex.hh"

#include <cstddef>

namespace sl {

VertexLayout Vertex3::layout() {
    return {
        .stride = sizeof(Vertex3),
        .attributes = {
            {0, ValueType::f32_3x1, offsetof(Vertex3, position)},
            {1, ValueType::f32_3x1, offsetof(Vertex3, normal)},
            {2, ValueType::f32_2x1, offsetof(Vertex3, texCoord)},
        },
    };
}

}  // namespace sl
