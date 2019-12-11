#version 460 core

layout(location=0)in vec3 aPos;

//三个布局块  Shared  std140  packed
layout(std140)uniform Matrices
{
	mat4 projection;
	mat4 view;
};

//数据块 需要数据块类型 和 class名字 对应 out in 对应就好了
out VS_OUT
{
	vec2 TexCoords;
}vs_out;


uniform mat4 model;

void main()
{
	gl_Position=projection*view*model*vec4(aPos,1.);
	vs_out.TexCoords=aTexcoords;
}