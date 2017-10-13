#version 330 core

layout ( location = 0 ) in vec3 vertexPos;

uniform mat4 u_tModel;
uniform mat4 u_tView;
uniform mat4 u_tProj;

void main()
{
	gl_Position = u_tProj * u_tView * u_tModel * vec4( vertexPos, 1.0f );
}