#version 330 core

out vec4 fragColor;
in vec3 position;

uniform samplerCube skybox;
uniform vec3 cameraPosition;

vec3 normal = vec3(0.0f, 1.0f, 0.0f);
float alpha = 0.3f;

void main() {
    vec3 I = -normalize(cameraPosition - position);

    vec3 reflectFactor = reflect(I, normal);
    vec3 refractFactor = refract(I, normal, 1.0f / 1.33f);

    vec4 reflected_color = vec4(texture(skybox, reflectFactor).rgb, 1.0f);
    vec4 refracted_color = vec4(texture(skybox, refractFactor).rgb, 1.0f);

    fragColor = alpha * reflected_color + (1 - alpha) * refracted_color;
    fragColor.a = 0.4f;
}