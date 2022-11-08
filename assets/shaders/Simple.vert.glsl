#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;

layout(set = 0, binding = 0) uniform globalUniform_t {
    mat4 projection;
    mat4 view;
}
globalUniform;

void main() { gl_Position = globalUniform.projection * globalUniform.view * vec4(inPosition, 1.0); }
