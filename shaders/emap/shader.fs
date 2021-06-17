#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main() {
    vec3 viewDir = normalize(FragPos - cameraPos);
    vec3 normal = normalize(Normal);

    vec3 reflectDir = reflect(viewDir, normal);

    FragColor = vec4(texture(skybox, reflectDir).rgb, 1.0);
}
