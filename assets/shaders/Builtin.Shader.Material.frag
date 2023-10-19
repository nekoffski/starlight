// clang-format off
#version 450

layout (location = 0) out vec4 outColor;

layout (set = 1, binding = 0) uniform LocalUBO {
    vec4 diffuseColor;
    float shininess;
} localUBO;

const int diffuseMap  = 0;
const int specularMap = 1;
const int normalMap   = 3;

layout (set = 1, binding = 1) uniform sampler2D textures[2];

layout (location = 1) in struct DTO { 
    vec2 textureCoordinates; 
    vec3 normal;
    vec3 viewPosition;
    vec3 fragmentPosition;
    vec4 ambient;
} dto;

struct DirectionalLight {
    vec3 direction;
    vec4 color;
};

DirectionalLight light = {
    vec3(-0.57735, -0.57735, -0.57735),
    vec4(0.8, 0.8, 0.8, 1.0)
};

vec4 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection) {
    float diffuseFactor = max(dot(normal, -light.direction), 0.0);

    vec3 halfDirection = normalize(viewDirection - light.direction);
    float specularFactor = pow(max(dot(halfDirection, normal), 0.0), localUBO.shininess);

    vec4 diffuseTextureSample = texture(textures[diffuseMap], dto.textureCoordinates);
    vec4 specularTextureSample = vec4(texture(
        textures[specularMap], dto.textureCoordinates).rgb, diffuseTextureSample.a);

    vec4 ambient = vec4(vec3(localUBO.diffuseColor * dto.ambient), diffuseTextureSample.a);
    vec4 diffuse = vec4(vec3(light.color * diffuseFactor), diffuseTextureSample.a);
    vec4 specular = vec4(vec3(light.color * specularFactor), diffuseTextureSample.a);

    return diffuseTextureSample * (diffuse + ambient) + specular * specularTextureSample;
}

void main() { 
    vec3 viewDirection = normalize(dto.viewPosition - dto.fragmentPosition);
    outColor = calculateDirectionalLight(light, dto.normal, viewDirection);
}
