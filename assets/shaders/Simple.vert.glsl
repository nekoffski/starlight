#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 0) out vec3 outPosition;

layout(set = 0, binding = 0) uniform globalUniform_t {
    mat4 projection;
    mat4 view;
}
globalUniform;

layout(push_constant) uniform pushConstants_t { mat4 model; }
pushConstants;

void main() {
    outPosition = inPosition;
    gl_Position =
        globalUniform.projection * globalUniform.view * pushConstants.model * vec4(inPosition, 1.0);
}
