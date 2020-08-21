#version 460 core
layout(location=0)in vec3 aPos;
uniform mat4 projection;
uniform mat4 view;

out vec3 WorldPos;

void main()
{
	WorldPos=aPos;
	
	mat4 rotView=mat4(mat3(view));//去掉位移 当作天空盒
	vec4 clipPos=projection*rotView*vec4(WorldPos,1.);
	
	gl_Position=clipPos.xyww;//当作最远处 w是1
}