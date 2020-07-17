#version 460 core
layout(location=0)in vec2 position;
layout(location=1)in vec2 texcoords;

out vec2 TexCoords;

void main()
{
	gl_Position=vec4(position,0.f,1.f);
	TexCoords=texcoords;
}