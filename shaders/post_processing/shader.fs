#version 330 core
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D sampler;

void main() {
    FragColor = vec4(1.0 - vec3(texture(sampler, TexCoords)), 1.0);
}
