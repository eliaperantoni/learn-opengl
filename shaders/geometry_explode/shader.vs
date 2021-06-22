#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out I_v2g {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} i_out;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    i_out.FragPos   = vec3(model * vec4(aPos, 1.0));
    i_out.Normal    = transpose(inverse(mat3(model))) * aNormal;
    i_out.TexCoords = aTexCoords;
}
