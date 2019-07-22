#version 460 core

out vec4 fragColor;

in vec2 texcoord;

uniform sampler2D mainTexture;

void main()
{
	fragColor=texture(mainTexture,vec2(texcoord.x,texcoord.y));
}