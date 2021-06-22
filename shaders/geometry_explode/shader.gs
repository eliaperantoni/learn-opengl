#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in I_v2g {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} i_in[];

out I_g2f {
    vec2 TexCoords;
} i_out;

uniform float time;

vec3 getNormal() {
    vec3 a = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);

    return normalize(cross(b, a));
}

vec4 doOffset(vec4 pos) {
    return pos + vec4(getNormal(), 0.0) * 6.0 * (sin(time) + 1) * 0.5;
}

void main() {
    gl_Position = doOffset(gl_in[0].gl_Position);
    i_out.TexCoords = i_in[0].TexCoords;
    EmitVertex();

    gl_Position = doOffset(gl_in[1].gl_Position);
    i_out.TexCoords = i_in[1].TexCoords;
    EmitVertex();

    gl_Position = doOffset(gl_in[2].gl_Position);
    i_out.TexCoords = i_in[2].TexCoords;
    EmitVertex();

    EndPrimitive();
}
