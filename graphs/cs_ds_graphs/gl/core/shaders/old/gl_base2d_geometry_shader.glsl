#version 330 core

layout(points) in;
layout(points) out;

void main()
{
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    EndPrimitive();
}