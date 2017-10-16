#version 330 core

uniform vec4 u_color;// unused

in vec3 vColor;
out vec4 color;

void main()
{
	color = vec4( vColor, 1.0 );
}