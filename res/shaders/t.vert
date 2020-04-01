#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 aTexPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 texPos;

void main() {
    texPos = aTexPos;
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
