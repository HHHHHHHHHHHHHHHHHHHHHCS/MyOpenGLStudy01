#version 460 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec2 aTexcoord;

out vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//乘法要从右向左边读
	gl_Position=projection*view*model*vec4(aPos,1);
	texcoord=aTexcoord;
}