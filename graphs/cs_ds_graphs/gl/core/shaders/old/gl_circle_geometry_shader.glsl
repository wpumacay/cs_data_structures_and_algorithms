#version 330 core

layout(points) in;
layout(line_strip, max_vertices = 41) out;

uniform float u_cRadius;

const float PI = 3.1415926;

void main()
{

    for (int i = 0; i <= 40; i++) {
        // Angle between each side in radians
        float ang = PI * 2.0 / 40.0 * i;

        // Offset from center of point (0.3 to accomodate for aspect ratio)
        vec4 offset = vec4( cos( ang ) * u_cRadius, 
                            -sin( ang ) * u_cRadius,
                            0.0, 0.0 );
        gl_Position = gl_in[0].gl_Position + offset;

        EmitVertex();
    }

    EndPrimitive();
}