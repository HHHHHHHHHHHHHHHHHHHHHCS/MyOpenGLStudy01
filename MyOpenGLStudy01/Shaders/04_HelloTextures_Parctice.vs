#version 330 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aColor;
layout(location=2)in vec2 aTexcoord;

out vec3 outColor;
out vec2 texcoord;

void main()
{
	gl_Position=vec4(aPos,1.);
	outColor=aColor;
	texcoord=aTexcoord;
}