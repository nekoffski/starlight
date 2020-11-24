#version 330 core

layout(location = 0) in vec3 iPos;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(iPos, 1.0f);
}
