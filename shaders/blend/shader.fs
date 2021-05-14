#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

struct Material {
    sampler2D texture_diffuse1;
};
uniform Material material;

uniform sampler2D sampler;

out vec4 FragColor;

void main() {
    FragColor = texture(material.texture_diffuse1, TexCoords);
}
