#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec3 inPosition;

void main() { outColor = vec4(inPosition.x + 0.5, inPosition.y + 0.5, inPosition.z + 0.5, 1.0); }
