#version 460 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec2 aTexcoords;

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 Texcoords;
}vs_out;

uniform mat4 Model;
uniform mat4 NormalMatrix;
uniform mat4 MVP;

void main()
{
	vs_out.FragPos=(Model*vec4(aPos,1.)).xyz;
	vs_out.Normal=mat3(NormalMatrix)*aNormal;
	vs_out.Texcoords=aTexcoords;
	gl_Position=MVP*vec4(aPos,1.);
}