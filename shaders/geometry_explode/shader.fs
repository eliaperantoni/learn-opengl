#version 330 core

in I_g2f {
    vec2 TexCoords;
} i_in;

struct Material {
    sampler2D texture_diffuse1;
};
uniform Material material;

out vec4 FragColor;

void main() {
    vec4 color = texture(material.texture_diffuse1, i_in.TexCoords);
    if(color.a < 0.1) {
        discard;
    }

    FragColor = color;
}
