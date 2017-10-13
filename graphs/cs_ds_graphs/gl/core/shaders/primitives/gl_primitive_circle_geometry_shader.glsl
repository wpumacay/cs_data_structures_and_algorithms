#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 61) out;

uniform float u_cRadius;

const float PI = 3.1415926;

void main()
{

    for ( int i = 0; i <= 20; i++ ) 
    {
        // Angle between each side in radians
        float _ang_1 = PI * 2.0 / 20.0 * i;
        float _ang_2 = PI * 2.0 / 20.0 * ( i + 1 );

        // Offset from center of point (0.3 to accomodate for aspect ratio)
        vec4 _offset_1 = vec4( cos( _ang_1 ) * u_cRadius, 
                               -sin( _ang_1 ) * u_cRadius,
                               0.0, 0.0 );

        vec4 _offset_2 = vec4( cos( _ang_2 ) * u_cRadius,
                               -sin( _ang_2 ) * u_cRadius,
                               0.0, 0.0 );

        // Center
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();

        // V1
        gl_Position = gl_in[0].gl_Position + _offset_1;
        EmitVertex();

        // V2
        gl_Position = gl_in[0].gl_Position + _offset_2;
        EmitVertex();
    }

    EndPrimitive();
}