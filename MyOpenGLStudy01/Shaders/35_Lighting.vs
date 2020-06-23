#version 460 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec2 aTexCoords;

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
}vs_out;

uniform mat4 viewProjection;
uniform mat4 model;

void main()
{
	vs_out.FragPos=vec3(model*vec4(aPos,1.));
	vs_out.TexCoords=aTexCoords;
	
	vec3 n=inverse_normals?-aNormal:aNormal;
	
	mat3 normalMatrix=transpose(inverse(mat3(model)));
	vs_out.Normal=normalize(normalMatrix*n);
	
	gl_Position=viewProjection*vec4(vs_out.FragPos.xyz,1.);
	
}