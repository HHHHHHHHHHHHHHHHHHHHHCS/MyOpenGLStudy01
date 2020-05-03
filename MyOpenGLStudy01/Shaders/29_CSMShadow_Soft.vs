#version 460 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec2 aTexcoords;

out vec2 Texcoords;

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 WorldPos;
}vs_out;

uniform mat4 viewProjection;
uniform mat4 view;
uniform mat4 model;


void main()
{
	vs_out.FragPos=vec3(model*vec4(aPos,1.));
	vs_out.Normal=transpose(inverse(mat3(model)))*aNormal;
	vs_out.TexCoords=aTexcoords;
	vs_out.WorldPos=model*vec4(aPos,1.);
	gl_Position=viewProjection*vs_out.WorldPos;
}
