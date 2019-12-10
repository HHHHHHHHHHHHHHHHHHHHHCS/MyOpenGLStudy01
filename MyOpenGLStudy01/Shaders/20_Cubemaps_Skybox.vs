#version 460 core

layout(location=0)in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	TexCoords=aPos;
	vec4 pos=projection*view*vec4(aPos,1.);
	//经过MVP w/w = 1   这里欺骗 z 深度 = 1
	gl_Position=pos.xyww;
}