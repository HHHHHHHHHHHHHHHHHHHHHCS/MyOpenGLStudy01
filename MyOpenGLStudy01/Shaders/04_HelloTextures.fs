#version 460 core

out vec4 fragColor;

in vec3 outColor;
in vec2 texcoord;

uniform sampler2D ourTexture;

void main()
{
	fragColor=texture(ourTexture,texcoord)*vec4(outColor,1);
}