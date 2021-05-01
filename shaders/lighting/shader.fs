#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};

uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

uniform vec3 viewPos;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main() {
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 norm     = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 viewDir  = normalize(viewPos - FragPos);

    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}
