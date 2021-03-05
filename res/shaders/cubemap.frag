#version 440 core

in vec3 tex_coords;
out vec4 frag_color;

uniform samplerCube skybox_sampler;

void main() {
    frag_color = texture(skybox_sampler, tex_coords);
}