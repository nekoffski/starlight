#version 330 core

layout(location = 0) in vec3 Lpos;

out vec3 tex_coords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    mat4 view_fixed = mat4(mat3(view));

    tex_coords = Lpos;
    vec4 sky_pos = projection * view_fixed * vec4(Lpos, 1.0f);
    gl_Position = sky_pos.xyww;
}