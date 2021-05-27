#version 460 core

layout(location=0)out vec4 frag;

uniform vec3 Color;

void main()
{
	frag=vec4(Color,1.f);
}