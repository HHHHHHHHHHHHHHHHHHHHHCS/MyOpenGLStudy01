#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
	FragColor=vec4(TexCoords.xy,0,1);
}