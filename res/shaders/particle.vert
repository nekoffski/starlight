#version 330 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNormal;
// layout(location = 2) in vec2 aTexPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 viewPos;

void main() {
    gl_Position = projection * view * vec4(iPos, 1.0f);
}
