#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 0) out vec3 outPosition;

void main() {
    outPosition = inPosition - vec3(0.25, 0, 0);
    gl_Position = vec4(outPosition, 1.0);
}
