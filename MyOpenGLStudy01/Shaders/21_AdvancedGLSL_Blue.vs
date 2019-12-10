#version 460 core

layout(location=0)in vec3 aPos;
/*
depth_<condition>
注意条件模式下 要GLSL Version>=420
any	默认值。提前深度测试是禁用的，你会损失很多性能
greater	你只能让深度值比gl_FragCoord.z更大
less	你只能让深度值比gl_FragCoord.z更小
unchanged	如果你要写入gl_FragDepth，你将只能写入gl_FragCoord.z的值
*/
layout(depth_greater)out float gl_FragDepth;

//三个布局块  Shared  std140  packed
layout(std140)uniform Matrices
{
	mat4 projection;
	mat4 view;
};

uniform mat4 model;

void main()
{
	gl_Position=projection*view*model*vec4(aPos,1.);
	gl_FragDepth=gl_FragCoord.z+.1;
}