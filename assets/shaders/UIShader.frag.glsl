#version 450

layout(location = 0) out vec4 out_colour;

layout(set = 1, binding = 0) uniform local_uniform_object { vec4 diffuse_colour; }
object_ubo;

// Samplers
layout(set = 1, binding = 1) uniform sampler2D diffuse_texture;

// Data Transfer Object
layout(location = 1) in struct dto { vec2 tex_coord; } in_dto;

void main() {
    object_ubo.diffuse_colour* texture(diffuse_texture, in_dto.tex_coord);
}