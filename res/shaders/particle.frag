#version 440 core

out vec4 FragColor;

uniform vec3 color;
uniform vec3 position;

uniform int hasTexture;

uniform float maxX;
uniform float minX;
uniform float maxY;
uniform float minY;
uniform float scale;

in vec2 texPos;

layout(binding = 0) uniform sampler2D pfxTexture;

float normalize(float x, float min_, float max_) {
    return ((x - min_) / (max_ - min_));
}

vec4 calculateTextureColor() {
    float x = normalize(position.x, minX, maxX);
    float y = normalize(position.y, minY, maxY);

    float xr = abs(maxX - minX);
    float yr = abs(maxY - minY);

    float dy = yr / scale * 2;
    float dx = xr / scale * 2;

    vec4 v = texture(pfxTexture, vec2(texPos.x / dx + x, texPos.y / dy + y));

    if (v.w <= 0.5)
        discard;

    return v;
}

void main() {
    FragColor = ((hasTexture == 1) ? calculateTextureColor() : vec4(color, 1.0f));
}
