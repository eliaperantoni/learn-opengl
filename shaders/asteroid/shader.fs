#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

struct Material {
    sampler2D texture_diffuse1;
};
uniform Material material;

out vec4 FragColor;

void main() {
    vec4 color = texture(material.texture_diffuse1, TexCoords);
    if(color.a < 0.1) {
        discard;
    }

    FragColor = color;
}
