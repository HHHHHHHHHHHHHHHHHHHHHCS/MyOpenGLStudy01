#version 460 core
layout(location=0)in vec3 aPos;

out vec3 WorldPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	WorldPos=aPos;//直接当作世界位置
	gl_Position=projection*view*vec4(WorldPos,1.);
}