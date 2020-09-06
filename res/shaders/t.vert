#version 330 core

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec2 iTexturePosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 texturePosition;
out vec3 fragmentPosition;
out vec3 normal;

void main() {
    texturePosition = iTexturePosition;
    normal = vec3(transpose(inverse(model)) * vec4(iNormal, 1.0f));
    fragmentPosition = vec3(model * vec4(iPosition, 1.0f));
    gl_Position = projection * view * model * vec4(iPosition, 1.0);
}
