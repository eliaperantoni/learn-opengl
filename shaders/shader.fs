#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    vec2 coord = 2 * TexCoord;
    FragColor = mix(texture(texture1, coord), texture(texture2, coord), 0.2);
}
