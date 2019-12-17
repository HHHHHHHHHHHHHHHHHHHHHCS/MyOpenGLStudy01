#version 460 core

layout(location=0)in vec3 aPos;
layout(location=1)in vec2 aTexcoords;

//三个布局块  Shared  std140  packed
layout(std140)uniform VPMatrices
{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

void main()
{
	gl_Position=projection*view*model*vec4(aPos,1.);
	//设置顶点渲染的方向区域像素大小
	gl_PointSize=20;//需要 glEnable(GL_PROGRAM_POINT_SIZE);
}