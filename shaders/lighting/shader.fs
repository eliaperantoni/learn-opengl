#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};
uniform Material material;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform SpotLight spotLight;

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

uniform vec3 viewPos;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main() {
    vec3 norm     = normalize(Normal);
    vec3 viewDir  = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);

    result += CalcDirLight(dirLight, norm, viewDir);
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    for(int i=0;i<NR_POINT_LIGHTS;i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    // Goes outwards
    vec3 lightDir = normalize(-light.direction);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0);

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);

    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));
    vec3 ambient = light.ambient * vec3(texture(material.specular, TexCoords));

    return diffuse + specular + ambient;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // Goes outwards
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0);

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);

    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));
    vec3 ambient = light.ambient;

    float theta = dot(normalize(-light.direction), lightDir);
    float epsilon = light.cutOff - light.outerCutOff;
    float strength = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float dist = length(light.position - fragPos);
    float attenuation = 1 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    diffuse *= strength;
    specular *= strength;
    ambient *= strength;

    diffuse *= attenuation;
    specular *= attenuation;
    ambient *= attenuation;

    return diffuse + specular + ambient;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // Goes outwards
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0);

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);

    // Attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));
    vec3 ambient = light.ambient;

    diffuse *= attenuation;
    specular *= attenuation;
    ambient *= attenuation;

    return diffuse + specular + ambient;
}
