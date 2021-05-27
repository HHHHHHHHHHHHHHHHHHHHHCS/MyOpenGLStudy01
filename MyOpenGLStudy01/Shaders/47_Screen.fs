#version 460 core

in vec2 texture_coords;

layout(location=0)out vec4 frag;

uniform sampler2D screen;

void main()
{
	frag=vec4(texture(screen,texture_coords).rgb,1.f);
}