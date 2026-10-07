#include "Vertex.hh"

namespace sl {

VertexLayout Vertex3::layout() {
    static VertexLayout layout{ValueType::f32_3x1};
    return layout;
}

}  // namespace sl
