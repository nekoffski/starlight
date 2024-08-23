#pragma once

#include <span>

#include "starlight/core/Core.hh"
#include "Vertex.hh"

namespace sl {

void generateFaceNormals(std::span<Vertex3> vertices, const std::span<u32> indices);
void generateTangents(std::span<Vertex3> vertices, const std::span<u32> indices);

}  // namespace sl
