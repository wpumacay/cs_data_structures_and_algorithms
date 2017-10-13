#version 330 core

layout(points) in;
layout(line_strip, max_vertices = 3) out;

uniform float u_dx;
uniform float u_dy;

void main()
{

    // p1
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    vec4 _offset = vec4( u_dx, u_dy, 0.0f, 0.0f );

    // p2
    gl_Position = gl_in[0].gl_Position + _offset;
    EmitVertex();

    EndPrimitive();
}