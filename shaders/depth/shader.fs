#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    float     shininess;
};
uniform Material material;

uniform vec3 viewPos;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

float near = 0.1;
float far  = 100.0;

float LinearizeDepth(float depth)
{
    // back to NDC, I think it is the NDC to screen space that maps depth from  [-1,+1] to [0,1]
    float z = depth * 2.0 - 1.0;

    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
    float depth = gl_FragCoord.z;
    depth = LinearizeDepth(depth) / far; // divide by far for demonstration
    FragColor = vec4(vec3(depth), 1.0);
}
