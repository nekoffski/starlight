// clang-format off

#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 in_texcoord;

layout(set = 0, binding = 0) uniform globalUniform_t {
    mat4 projection;
    mat4 view;
} globalUniform;

// layout(location = 0) out int out_mode;

// Data Transfer Object
layout(location = 1) out struct dto {
	vec2 tex_coord;
} out_dto;

layout(push_constant) uniform pushConstants_t { mat4 model; } pushConstants;

void main() {
    out_dto.tex_coord = in_texcoord;
    gl_Position =
        globalUniform.projection * globalUniform.view * pushConstants.model * vec4(inPosition, 1.0);
}
