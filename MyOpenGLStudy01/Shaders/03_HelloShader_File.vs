#version 460 core

layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aCol;

uniform float time;

out vec4 vertexColor;

void main()
{
	gl_Position=vec4(aPos,1.);
	vertexColor=vec4(aCol.xyz,1.)*(sin(time)+1)/2;
}