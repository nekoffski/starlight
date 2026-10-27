#include "Vertex.hh"

namespace sl {

VertexLayout Vertex3::layout() {
    return VertexLayout{
        ValueType::f32_3x1,
        ValueType::f32_3x1,
    };
}

}  // namespace sl
