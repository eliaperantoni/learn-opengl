#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceModel;

uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main() {
    gl_Position = projection * view * aInstanceModel * vec4(aPos, 1.0);

    FragPos   = vec3(aInstanceModel * vec4(aPos, 1.0));
    Normal    = transpose(inverse(mat3(aInstanceModel))) * aNormal;
    TexCoords = aTexCoords;
}
