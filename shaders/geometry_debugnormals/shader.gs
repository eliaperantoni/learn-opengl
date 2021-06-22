#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices=6) out;

in I_v2g {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} i_in[];

float MAGNITUDE = 0.2;

void drawNormal(int i) {
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(i_in[i].Normal, 0.0) * MAGNITUDE;
    EmitVertex();

    EndPrimitive();
}

void main() {
    drawNormal(0);
    drawNormal(1);
    drawNormal(2);
}
