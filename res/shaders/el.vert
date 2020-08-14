#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 aTexPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float t;

out vec3 position;

vec4 calculatePosition(vec4 pos) {
    vec3 k = vec3(1.0f, 1.0f, 1.0f);
    float omega = 5;

    pos.y = sin(t * omega + dot(k, vec3(pos)));

    return pos;
}

void main() {
    position = vec3(model * vec4(aPos, 1.0f));
    gl_Position = projection * view * model * calculatePosition(vec4(aPos.x, aPos.y, aPos.z, 1.0));
}
