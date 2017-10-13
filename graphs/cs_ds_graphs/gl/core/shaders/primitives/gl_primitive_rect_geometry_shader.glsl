#version 330 core

layout(points) in;
layout(line_strip, max_vertices = 5) out;

uniform float u_w;
uniform float u_h;
uniform float u_t;

const float PI = 3.1415926;

void main()
{

	float _d = 0.25 * sqrt( u_w * u_w + u_h * u_h );
	float _phi = atan( u_h / u_w );
	float _ang = 0;
	vec4 _offset = vec4( 0.0f, 0.0f, 0.0f, 0.0f );

	// p1
	_ang = u_t + _phi;
	_offset.x = _d * cos( _ang );
	_offset.y = _d * sin( _ang );
	gl_Position = gl_in[0].gl_Position + _offset;
	EmitVertex();

	// p2
	_ang = u_t + PI - _phi;
	_offset.x = _d * cos( _ang );
	_offset.y = _d * sin( _ang );
	gl_Position = gl_in[0].gl_Position + _offset;
	EmitVertex();

	// p3
	_ang = u_t + PI + _phi;
	_offset.x = _d * cos( _ang );
	_offset.y = _d * sin( _ang );
	gl_Position = gl_in[0].gl_Position + _offset;
	EmitVertex();

	// p4
	_ang = u_t + 2 * PI - _phi;
	_offset.x = _d * cos( _ang );
	_offset.y = _d * sin( _ang );
	gl_Position = gl_in[0].gl_Position + _offset;
	EmitVertex();


    EndPrimitive();
}