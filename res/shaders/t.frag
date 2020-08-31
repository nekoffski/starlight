#version 330 core

out vec4 FragColor;

in vec2 texPos;
in vec3 norm;
in vec3 pos;


uniform sampler2D textureSampler;

const float ambientFactor = 0.2f;
const float specularFactor = 0.5f;

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

uniform DirectionalLights directionalLights[5];
uniform PointLights pointLights[5];
uniform Material material;
uniform vec3 viewPos;

vec4 calculateDirectionalLights() {
    DirectionalLights light = directionalLights[0];

    vec3 ambient = light.color * ambientFactor * material.ambientColor;
    
    float diff = max(dot(norm, normalize(light.direction)), 0.0f);
    vec3 diffuse = light.color * diff * material.diffuseColor;

    // shouldnt it be transforem to world coords?
    vec3 viewDir = normalize(viewPos - pos);
    vec3 reflected = normalize(reflect(light.direction, norm));

    float spec = pow(max(dot(viewDir, reflected), 0.0f), material.shininess);

    vec3 specular = specularFactor * spec * light.color * material.specularColor;

    return vec4(ambient + diffuse + specular, 1.0f);
}


float attenuation(float d, float a, float b, float c) {
    return 1.0f / (c + d * b + d * d * a);
}

vec4 calculatePointLights() {
    PointLights light = pointLights[0];

    vec3 position = vec3(light.modelMatrix * vec4(light.position, 1.0f));
    vec3 ambient = light.color * ambientFactor * material.ambientColor;
    vec3 lightDir = normalize(position - pos);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.color * diff * material.diffuseColor;

    vec3 viewDir = normalize(viewPos - pos);
    vec3 reflected = normalize(reflect(lightDir, norm));

    float spec = pow(max(dot(viewDir, reflected), 0.0f), material.shininess);

    vec3 specular = specularFactor * spec * light.color * material.specularColor;

    float d = length(position - pos);
    float att = attenuation(d, light.attenuationA, light.attenuationB, light.attenuationC);

    return vec4((ambient + diffuse + specular) * att, 1.0f);
}


vec4 calculateColor() {
    return calculateDirectionalLights() + calculatePointLights();
}

void main() {
    FragColor = texture(textureSampler, texPos) * calculateColor();
}