#version 330 core
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D sampler;

void main() {
    FragColor = texture(sampler, TexCoords);
    float avg = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    FragColor = vec4(avg, avg, avg, 1.0);
}
