#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main() {
    float refractionRatio = 1.00 / 1.52;

    vec3 viewDir = normalize(FragPos - cameraPos);
    vec3 normal = normalize(Normal);

    vec3 refractDir = refract(viewDir, normal, refractionRatio);

    FragColor = vec4(texture(skybox, refractDir).rgb, 1.0);
}
