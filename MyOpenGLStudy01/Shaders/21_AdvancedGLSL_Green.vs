#version 460 core

layout(location=0)in vec3 aPos;
layout(location=1)in vec2 aTexcoords;

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
	//单片元点的尺寸
	gl_PointSize=4;//需要 glEnable(GL_PROGRAM_POINT_SIZE);
}