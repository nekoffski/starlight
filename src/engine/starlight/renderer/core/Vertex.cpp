#include "Vertex.hh"

namespace sl {

VertexLayout Vertex3::layout() {
    return VertexLayout{
        DataFormat::f32_3x1,
        DataFormat::f32_3x1,
    };
}

}  // namespace sl
