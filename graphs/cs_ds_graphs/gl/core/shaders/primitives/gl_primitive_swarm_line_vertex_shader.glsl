#version 330 core

layout ( location = 0 ) in vec3 vertexPos;
layout ( location = 1 ) in vec3 vertexColor;

uniform mat4 u_tModel;
uniform mat4 u_tView;
uniform mat4 u_tProj;

out vec3 vColor;

void main()
{
	gl_Position = u_tProj * u_tView * u_tModel * vec4( vertexPos, 1.0f );
	vColor = vertexColor;
}