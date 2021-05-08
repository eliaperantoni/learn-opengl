#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main() {
    FragColor = vec4(0.0, 0.2, 1.0, 1.0);
}
