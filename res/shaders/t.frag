#version 330 core

out vec4 FragColor;

in vec2 texturePosition;
in vec3 normal;
in vec3 fragmentPosition;

struct DirectionalLights {
    vec3 direction;
    vec3 color;
};

struct PointLights {
    mat4 modelMatrix;
    vec3 position;
    vec3 color;
    float attenuationA;
    float attenuationB;
    float attenuationC;
};

struct Material {
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};

#define MAX_DIRECTIONAL_LIGHTS 64
#define MAX_POINT_LIGHTS 64

uniform sampler2D textureSampler;
uniform unsigned int directionalLightsNum;
uniform unsigned int pointLightsNum;
uniform DirectionalLights directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLights pointLights[MAX_POINT_LIGHTS];
uniform Material material;
uniform vec3 viewPos;

float calculateAttenuation(float fragmentDistance, float a, float b, float c) {
    return 1.0f / (c + fragmentDistance * b + fragmentDistance * fragmentDistance * a);
}

vec3 calculateAmbient(vec3 lightColor) {
    const float ambientFactor = 0.2f;
    return lightColor * ambientFactor * material.ambientColor;
}

vec3 calculateDiffuse(vec3 lightDirection, vec3 lightColor) {
    float diff = max(dot(normal, normalize(lightDirection)), 0.0f);
    return lightColor * diff * material.diffuseColor;
}

vec3 calculateSpecular(vec3 viewDirection, vec3 lightDirection, vec3 lightColor) {
    const float specularFactor = 0.5f;

    vec3 reflected = normalize(reflect(lightDirection, normal));
    float spec = pow(max(dot(viewDirection, reflected), 0.0f), material.shininess);
    return specularFactor * spec * lightColor * material.specularColor;
}

vec4 calculateDirectionalLights(vec3 viewDirection) {
    vec3 lightSum = vec3(0.0f);

    for (unsigned int i = 0u; i < directionalLightsNum; ++i) {
        DirectionalLights light = directionalLights[i];

        vec3 ambient = calculateAmbient(light.color);
        vec3 diffuse = calculateDiffuse(light.direction, light.color);
        vec3 specular = calculateSpecular(viewDirection, light.direction, light.color);

        lightSum += (ambient + diffuse + specular);
    }
    return vec4(lightSum, 1.0f);
}

vec4 calculatePointLights(vec3 viewDirection) {
    vec3 lightSum = vec3(0.0f);

    for (unsigned int i = 0u; i < pointLightsNum; ++i) {
        PointLights light = pointLights[0];
        vec3 lightPosition = vec3(light.modelMatrix * vec4(light.position, 1.0f));
        vec3 lightDirection = normalize(lightPosition - fragmentPosition);

        vec3 ambient = calculateAmbient(light.color);
        vec3 diffuse = calculateDiffuse(lightDirection, light.color);
        vec3 specular = calculateSpecular(viewDirection, lightDirection, light.color);

        float fragmentDistance = length(lightPosition - fragmentPosition);
        float attenuationFactor = calculateAttenuation(fragmentDistance, light.attenuationA, light.attenuationB, light.attenuationC);

        lightSum += (ambient + diffuse + specular) * attenuationFactor;
    }

    return vec4(lightSum, 1.0f);
}

vec4 calculateLight() {
    vec3 viewDirection = normalize(viewPos - fragmentPosition);
    return calculateDirectionalLights(viewDirection) + calculatePointLights(viewDirection);
}

void main() {
    FragColor = texture(textureSampler, texturePosition) * calculateLight();
}
