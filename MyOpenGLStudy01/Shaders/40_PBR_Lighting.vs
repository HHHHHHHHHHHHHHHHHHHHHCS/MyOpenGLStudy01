#version 460 core

layout(location=0)in vec3 aPos;
layout(location=1)in vec2 aTexCoords;
layout(location=2)in vec3 aNormal;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	TexCoords=aTexCoords;
	WorldPos=vec3(model*vec4(aPos,1.));
	Normal=mat3(model)*anormal;//这里是统一缩放的  所以不用逆矩阵
	
	gl_Postion=projection*view*vec4(WorldPos,1.);
}