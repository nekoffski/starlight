#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 aTexPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 texPos;
out vec3 pos;
out vec3 norm;

void main() {
    texPos = aTexPos;
    norm = vec3(transpose(inverse(model)) * vec4(normals, 1.0f));
    pos = vec3(model * vec4(aPos, 1.0f));
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
